#include "Application.h"

#include "Clock.h"
#include "Logger.h"
#include "PlatformLayer.h"

#include <Renderer/Pipeline.h>
#include <Renderer/Renderer.h>
#include <array>
#include <string>

#pragma warning(disable : 26495)
struct ApplicationState
{
	bool    m_IsRunning;
	bool    m_IsSuspended;
	int16_t m_Width;
	int16_t m_Height;

	Fatal::Clock m_Clock;
	double       m_LastTime;

	Platform::PlatformState m_PlatformState;
	Application::Game       m_Game;
};

static ApplicationState app_state;
static bool             is_initialized = false;

namespace Application
{
using enum Logger::Level;

bool create_application(Game const& game)
{
	if(is_initialized)
	{
		Logger::log<Error>("App created more than once.");
		return false;
	}

	app_state.m_Game = game;

	// Subsystems initialization
	Logger::initialize_logger();

	app_state.m_IsRunning   = true;
	app_state.m_IsSuspended = false;

	if(!app_state.m_PlatformState.start_platform(game.m_Config.m_AppName, game.m_Config.m_StartPosX,
		   game.m_Config.m_StartPosY, game.m_Config.m_StartWidth,
		   game.m_Config.m_StartHeight))
	{
		return false;
	}

	if(!app_state.m_Game.initialize())
	{
		Logger::log<Fatal>("Game initialization failed.");
		return false;
	}

	is_initialized = true;
	return true;
}

bool run_application()
{
	app_state.m_Clock.start();
	app_state.m_Clock.update();
	app_state.m_LastTime = app_state.m_Clock.get_elapsed_time();

	auto ctx =
		create_context(app_state.m_Game.m_Config.m_AppName, app_state.m_PlatformState.get_state(),
			app_state.m_Game.m_Config.m_StartWidth + 100, app_state.m_Game.m_Config.m_StartHeight + 100);

	const auto& device            = ctx.m_VulkanDevice.m_Device;
	const auto [acquire, release] = Utils::Swapchain::create_semaphores(device);

	auto     props = Utils::Device::get_queue_family_properties(ctx.m_VulkanDevice.m_PhysicalDevice);
	uint32_t index = Utils::Device::select_queue_index(props);

	VkFenceCreateInfo fence_create_info = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
	};

	VkFence fence;
	vkCreateFence(ctx.m_VulkanDevice.m_Device, &fence_create_info, nullptr, &fence);

	CommandPool   cmd_pool(ctx.m_VulkanDevice.m_Device, index);
	CommandBuffer cmd_buffer(ctx.m_VulkanDevice.m_Device, cmd_pool.get(), CommandBufferLevel::Primary);

	const auto& cb = cmd_buffer.get_buffer();

	auto vert = GLSL::create_shader(device, "C:/Dev/cpp/FatalEngine/Shaders/Default.vert.spv");
	auto frag = GLSL::create_shader(device, "C:/Dev/cpp/FatalEngine/Shaders/Default.frag.spv");

	const auto descriptor_layout = Utils::Descriptor::create_descriptor_layout(device);

	PipelineBuilder pipeline = PipelineBuilder()
	                               .add_shader(ShaderType::Vertex, vert)
	                               .add_shader(ShaderType::Fragment, frag)
	                               .init_viewport(app_state.m_Game.m_Config.m_StartWidth, app_state.m_Game.m_Config.m_StartHeight)
	                               .add_descriptor_set_layout(descriptor_layout);
	pipeline.build(device, ctx.m_VulkanSwapchain.m_RenderPass);

	double  running_time         = 0.0;
	uint8_t frame_count          = 0;
	double  target_frame_seconds = 1.0 / 60.0;

	while(app_state.m_IsRunning)
	{
		if(!app_state.m_PlatformState.pump_messages())
		{
			app_state.m_IsRunning = false;
		}

		if(!app_state.m_IsSuspended)
		{
			app_state.m_Clock.update();
			double current_time     = app_state.m_Clock.get_elapsed_time();
			double dt               = current_time - app_state.m_LastTime;
			double frame_start_time = Platform::get_time();

			if(!app_state.m_Game.update(dt))
			{
				Logger::log<Fatal>("Game update failed.");
				app_state.m_IsRunning = false;
				break;
			}

			// Render
			uint32_t img_index = 0;
			vkAcquireNextImageKHR(ctx.m_VulkanDevice.m_Device, ctx.m_VulkanSwapchain.m_Swapchain, UINT64_MAX, acquire,
				0, &img_index);
			vkDeviceWaitIdle(ctx.m_VulkanDevice.m_Device);
			vkWaitForFences(ctx.m_VulkanDevice.m_Device, 1, &fence, VK_TRUE, UINT64_MAX);
			vkResetFences(ctx.m_VulkanDevice.m_Device, 1, &fence);

			cmd_pool.reset(ctx.m_VulkanDevice.m_Device);
			cmd_buffer.begin();

			Utils::Swapchain::RenderPass::begin_render_pass(img_index, ctx, cb,
				app_state.m_Game.m_Config.m_StartWidth,
				app_state.m_Game.m_Config.m_StartHeight);
			pipeline.bind(cb);
			vkCmdDraw(cb, 3, 1, 0, 0);
			vkCmdEndRenderPass(cb);
			cmd_buffer.end();

			Utils::Device::submit_queue(ctx.m_VulkanDevice.m_Queue, cb, fence, acquire, release);
			Utils::Device::queue_present(ctx, release, img_index);

			if(!app_state.m_Game.render(dt))
			{
				Logger::log<Fatal>("Game render failed.");
				app_state.m_IsRunning = false;
				break;
			}

			double frame_end_time     = Platform::get_time();
			double frame_elapsed_time = frame_end_time - frame_start_time;
			running_time += frame_elapsed_time;

			double remaining_seconds = target_frame_seconds - frame_elapsed_time;

			if(remaining_seconds > 0)
			{
				double remaining_ms = remaining_seconds * 1000.0;

				bool limit_frames = false;
				if(remaining_ms > 0 && limit_frames)
				{
					Platform::sleep(static_cast<unsigned long>(remaining_ms - 1));
				}

				frame_count++;
			}

			app_state.m_LastTime = current_time;
		}
	}
	app_state.m_IsRunning = false;
	return true;
}
} // namespace Application
