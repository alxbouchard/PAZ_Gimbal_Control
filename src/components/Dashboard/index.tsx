import { useMemo } from 'react';
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  ResponsiveContainer,
  Legend,
} from 'recharts';
import { Activity, Thermometer, Battery, Clock, Target } from 'lucide-react';
import { useGimbalStore } from '../../store/gimbalStore';

interface StatCardProps {
  label: string;
  value: string | number;
  unit?: string;
  icon: React.ReactNode;
  color?: string;
}

function StatCard({ label, value, unit, icon, color = 'gimbal-accent' }: StatCardProps) {
  return (
    <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-4">
      <div className="flex items-center gap-3">
        <div className={`p-2 rounded-lg bg-${color}/20 text-${color}`}>
          {icon}
        </div>
        <div>
          <p className="text-xs text-gimbal-text-dim">{label}</p>
          <p className="text-xl font-semibold text-gimbal-text">
            {value}
            {unit && <span className="text-sm text-gimbal-text-dim ml-1">{unit}</span>}
          </p>
        </div>
      </div>
    </div>
  );
}

export function Dashboard() {
  const { position, telemetryHistory, connected, tracking, speedBoost } = useGimbalStore();

  // Format telemetry data for charts
  const chartData = useMemo(() => {
    return telemetryHistory.map((data, index) => ({
      time: index,
      pitch: data.position.pitch,
      yaw: data.position.yaw,
      roll: data.position.roll,
      temperature: data.temperature,
    }));
  }, [telemetryHistory]);

  // Generate demo data if no telemetry
  const displayData = useMemo(() => {
    if (chartData.length > 0) return chartData;

    // Generate smooth demo data
    return Array.from({ length: 50 }, (_, i) => ({
      time: i,
      pitch: Math.sin(i * 0.1) * 20 + position.pitch,
      yaw: Math.cos(i * 0.08) * 30 + position.yaw,
      roll: Math.sin(i * 0.12) * 10 + position.roll,
      temperature: 35 + Math.random() * 5,
    }));
  }, [chartData, position]);

  const CustomTooltip = ({ active, payload, label }: any) => {
    if (active && payload && payload.length) {
      return (
        <div className="bg-gimbal-panel border border-gimbal-border rounded-lg p-3 text-xs">
          <p className="text-gimbal-text-dim mb-2">Time: {label}</p>
          {payload.map((entry: any, index: number) => (
            <p key={index} style={{ color: entry.color }}>
              {entry.name}: {entry.value.toFixed(1)}°
            </p>
          ))}
        </div>
      );
    }
    return null;
  };

  return (
    <div className="space-y-6">
      <div className="flex items-center gap-2">
        <Activity size={20} className="text-gimbal-accent" />
        <h2 className="text-lg font-semibold text-gimbal-text">Dashboard</h2>
      </div>

      {/* Stats Grid */}
      <div className="grid grid-cols-2 md:grid-cols-4 gap-4">
        <StatCard
          label="Pitch"
          value={position.pitch.toFixed(1)}
          unit="°"
          icon={<Target size={18} />}
          color="gimbal-accent"
        />
        <StatCard
          label="Yaw"
          value={position.yaw.toFixed(1)}
          unit="°"
          icon={<Target size={18} />}
          color="gimbal-success"
        />
        <StatCard
          label="Roll"
          value={position.roll.toFixed(1)}
          unit="°"
          icon={<Target size={18} />}
          color="gimbal-warning"
        />
        <StatCard
          label="Status"
          value={connected ? 'Online' : 'Offline'}
          icon={<Activity size={18} />}
          color={connected ? 'gimbal-success' : 'gimbal-error'}
        />
      </div>

      {/* Mode indicators */}
      <div className="flex gap-4">
        <div
          className={`flex items-center gap-2 px-4 py-2 rounded-lg border ${
            tracking
              ? 'bg-gimbal-success/20 border-gimbal-success text-gimbal-success'
              : 'bg-gimbal-panel border-gimbal-border text-gimbal-text-dim'
          }`}
        >
          <Target size={16} />
          <span className="text-sm">Tracking {tracking ? 'ON' : 'OFF'}</span>
        </div>
        <div
          className={`flex items-center gap-2 px-4 py-2 rounded-lg border ${
            speedBoost
              ? 'bg-gimbal-warning/20 border-gimbal-warning text-gimbal-warning'
              : 'bg-gimbal-panel border-gimbal-border text-gimbal-text-dim'
          }`}
        >
          <Activity size={16} />
          <span className="text-sm">Speed Boost {speedBoost ? 'ON' : 'OFF'}</span>
        </div>
      </div>

      {/* Position Chart */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4">Position History</h3>
        <div className="h-64">
          <ResponsiveContainer width="100%" height="100%">
            <LineChart data={displayData}>
              <CartesianGrid strokeDasharray="3 3" stroke="#1e1e2e" />
              <XAxis
                dataKey="time"
                stroke="#64748b"
                tick={{ fill: '#64748b', fontSize: 10 }}
              />
              <YAxis
                stroke="#64748b"
                tick={{ fill: '#64748b', fontSize: 10 }}
                domain={[-90, 90]}
              />
              <Tooltip content={<CustomTooltip />} />
              <Legend
                wrapperStyle={{ fontSize: '12px' }}
                iconType="line"
              />
              <Line
                type="monotone"
                dataKey="pitch"
                stroke="#3b82f6"
                strokeWidth={2}
                dot={false}
                name="Pitch"
              />
              <Line
                type="monotone"
                dataKey="yaw"
                stroke="#22c55e"
                strokeWidth={2}
                dot={false}
                name="Yaw"
              />
              <Line
                type="monotone"
                dataKey="roll"
                stroke="#f59e0b"
                strokeWidth={2}
                dot={false}
                name="Roll"
              />
            </LineChart>
          </ResponsiveContainer>
        </div>
      </div>

      {/* Secondary Stats */}
      <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
        <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-4">
          <div className="flex items-center gap-3 mb-3">
            <Thermometer size={18} className="text-gimbal-warning" />
            <span className="text-sm text-gimbal-text-dim">Temperature</span>
          </div>
          <div className="flex items-end gap-2">
            <span className="text-2xl font-semibold text-gimbal-text">38</span>
            <span className="text-sm text-gimbal-text-dim mb-1">°C</span>
          </div>
          <div className="mt-2 h-2 bg-gimbal-bg rounded-full overflow-hidden">
            <div
              className="h-full bg-gradient-to-r from-gimbal-success via-gimbal-warning to-gimbal-error"
              style={{ width: '45%' }}
            />
          </div>
        </div>

        <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-4">
          <div className="flex items-center gap-3 mb-3">
            <Battery size={18} className="text-gimbal-success" />
            <span className="text-sm text-gimbal-text-dim">Battery</span>
          </div>
          <div className="flex items-end gap-2">
            <span className="text-2xl font-semibold text-gimbal-text">85</span>
            <span className="text-sm text-gimbal-text-dim mb-1">%</span>
          </div>
          <div className="mt-2 h-2 bg-gimbal-bg rounded-full overflow-hidden">
            <div className="h-full bg-gimbal-success" style={{ width: '85%' }} />
          </div>
        </div>

        <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-4">
          <div className="flex items-center gap-3 mb-3">
            <Clock size={18} className="text-gimbal-accent" />
            <span className="text-sm text-gimbal-text-dim">Uptime</span>
          </div>
          <div className="flex items-end gap-2">
            <span className="text-2xl font-semibold text-gimbal-text">2h 34m</span>
          </div>
          <p className="text-xs text-gimbal-text-dim mt-2">
            Session started at 10:30 AM
          </p>
        </div>
      </div>
    </div>
  );
}
