#pragma once

#include "plugins/telemetry_server/telemetry_server.h"
#include "plugin_impl_base.h"

#include <unordered_map>
#include <chrono>

namespace mavsdk {

class TelemetryServerImpl : public PluginImplBase {
public:
    struct RequestMsgInterval {
        uint32_t msg_id{0};
        uint32_t interval{0};
        void* cookie{nullptr};
    };

    explicit TelemetryServerImpl(System& system);
    explicit TelemetryServerImpl(std::shared_ptr<System> system);
    ~TelemetryServerImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    void publish_position_async(
        TelemetryServer::Position position,
        TelemetryServer::VelocityNed velocity_ned,
        const TelemetryServer::ResultCallback callback);

    TelemetryServer::Result
    publish_position(TelemetryServer::Position position, TelemetryServer::VelocityNed velocity_ned);

    TelemetryServer::Result publish_home(TelemetryServer::Position home);

    TelemetryServer::Result
    publish_raw_gps(TelemetryServer::RawGps raw_gps, TelemetryServer::GpsInfo gps_info);

    TelemetryServer::Result publish_battery(TelemetryServer::Battery battery);

    TelemetryServer::Result publish_status_text(TelemetryServer::StatusText status_text);

    TelemetryServer::Result publish_odometry(TelemetryServer::Odometry odometry);

    TelemetryServer::Result
    publish_position_velocity_ned(TelemetryServer::PositionVelocityNed position_velocity_ned);

    TelemetryServer::Result publish_ground_truth(TelemetryServer::GroundTruth ground_truth);

    void
    publish_imu_async(TelemetryServer::Imu imu, const TelemetryServer::ResultCallback callback);

    TelemetryServer::Result publish_imu(TelemetryServer::Imu imu);

    TelemetryServer::Result publish_scaled_imu(TelemetryServer::Imu imu);

    TelemetryServer::Result publish_raw_imu(TelemetryServer::Imu imu);

    TelemetryServer::Result publish_health_all_ok(bool is_health_all_ok);

    TelemetryServer::Result publish_unix_epoch_time(uint64_t time_us);

    TelemetryServer::Result publish_sys_status(
        TelemetryServer::Battery battery,
        bool rc_receiver_status,
        bool gyro_status,
        bool accel_status,
        bool mag_status,
        bool gps_status);

    TelemetryServer::Result publish_extended_sys_state(
        TelemetryServer::VtolState vtol_state, TelemetryServer::LandedState landed_state);

private:
    std::chrono::time_point<std::chrono::steady_clock> _start_time;

    std::vector<RequestMsgInterval> _interval_requests;
    std::mutex _interval_mutex;
    std::unordered_map<uint64_t, mavlink_message_t> _msg_cache;

    void add_msg_cache(uint64_t id, mavlink_message_t& msg);

    uint64_t get_boot_time_ms()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - _start_time)
            .count();
    }

    uint64_t get_epoch_time()
    {
        return std::chrono::steady_clock::now().time_since_epoch().count();
    }
};

} // namespace mavsdk