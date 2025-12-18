import { Info, Github, Mail, ExternalLink } from 'lucide-react';

export function AboutPage() {
  return (
    <div className="space-y-6 max-w-4xl">
      <div className="flex items-center gap-2">
        <Info size={20} className="text-gimbal-accent" />
        <h2 className="text-lg font-semibold text-gimbal-text">About</h2>
      </div>

      {/* Main Info */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <div className="flex items-start gap-6">
          <div className="w-20 h-20 rounded-2xl bg-gradient-to-br from-gimbal-accent to-purple-600 flex items-center justify-center flex-shrink-0">
            <svg
              viewBox="0 0 24 24"
              className="w-12 h-12 text-white"
              fill="none"
              stroke="currentColor"
              strokeWidth="1.5"
            >
              <circle cx="12" cy="12" r="3" />
              <circle cx="12" cy="12" r="8" strokeDasharray="4 2" />
              <line x1="12" y1="2" x2="12" y2="6" />
              <line x1="12" y1="18" x2="12" y2="22" />
              <line x1="2" y1="12" x2="6" y2="12" />
              <line x1="18" y1="12" x2="22" y2="12" />
            </svg>
          </div>

          <div>
            <h3 className="text-xl font-bold text-gimbal-text">
              PAZ Gimbal Control
            </h3>
            <p className="text-sm text-gimbal-text-dim mt-1">
              Professional Gimbal Management System
            </p>
            <div className="mt-4 flex items-center gap-4">
              <span className="px-3 py-1 bg-gimbal-bg rounded-full text-xs text-gimbal-text">
                Version 1.0.0
              </span>
              <span className="px-3 py-1 bg-gimbal-success/20 text-gimbal-success rounded-full text-xs">
                Latest
              </span>
            </div>
          </div>
        </div>

        <p className="mt-6 text-sm text-gimbal-text-dim leading-relaxed">
          PAZ Gimbal Control is a professional UI for controlling DJI gimbals and
          other compatible stabilizers. It provides real-time 3D visualization,
          intuitive controls, and comprehensive telemetry data.
        </p>
      </div>

      {/* Features */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4">Features</h3>

        <div className="grid grid-cols-2 gap-4">
          {[
            { title: '3D Visualization', desc: 'Real-time gimbal model' },
            { title: 'Virtual Gimbal', desc: 'Test without hardware' },
            { title: 'Joystick Controls', desc: 'Touch & mouse support' },
            { title: 'Telemetry Graphs', desc: 'Position history tracking' },
            { title: 'Gamepad Support', desc: 'Xbox controller compatible' },
            { title: 'Multi-Gimbal', desc: 'Control multiple devices' },
            { title: 'Keyboard Shortcuts', desc: 'Quick access controls' },
            { title: 'Custom Mapping', desc: 'Configurable inputs' },
          ].map((feature, index) => (
            <div key={index} className="p-3 bg-gimbal-bg rounded-lg">
              <div className="text-sm font-medium text-gimbal-text">
                {feature.title}
              </div>
              <div className="text-xs text-gimbal-text-dim">{feature.desc}</div>
            </div>
          ))}
        </div>
      </div>

      {/* Tech Stack */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4">
          Technology Stack
        </h3>

        <div className="flex flex-wrap gap-2">
          {[
            'React 18',
            'TypeScript',
            'Three.js',
            'React Three Fiber',
            'Framer Motion',
            'Tailwind CSS',
            'Zustand',
            'Socket.io',
            'Recharts',
            'Vite',
          ].map((tech) => (
            <span
              key={tech}
              className="px-3 py-1.5 bg-gimbal-bg border border-gimbal-border rounded-lg text-xs text-gimbal-text"
            >
              {tech}
            </span>
          ))}
        </div>
      </div>

      {/* Backend Info */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4">
          Backend Integration
        </h3>

        <p className="text-sm text-gimbal-text-dim mb-4">
          This UI is designed to work with the ZAP Tracking backend system:
        </p>

        <div className="space-y-2 text-sm">
          <div className="flex items-center gap-2">
            <span className="w-2 h-2 rounded-full bg-gimbal-accent" />
            <span className="text-gimbal-text">
              <strong>ZT_Lib</strong> - C++ gimbal control library
            </span>
          </div>
          <div className="flex items-center gap-2">
            <span className="w-2 h-2 rounded-full bg-gimbal-success" />
            <span className="text-gimbal-text">
              <strong>ZT_Agent</strong> - Background service daemon
            </span>
          </div>
          <div className="flex items-center gap-2">
            <span className="w-2 h-2 rounded-full bg-purple-500" />
            <span className="text-gimbal-text">
              <strong>DJI Protocol</strong> - EthCAN communication
            </span>
          </div>
          <div className="flex items-center gap-2">
            <span className="w-2 h-2 rounded-full bg-orange-500" />
            <span className="text-gimbal-text">
              <strong>ATEM Integration</strong> - Blackmagic switcher support
            </span>
          </div>
        </div>
      </div>

      {/* Credits */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4">Credits</h3>

        <div className="space-y-3">
          <div className="flex items-center justify-between">
            <div>
              <div className="text-sm text-gimbal-text">ZAP Tracking System</div>
              <div className="text-xs text-gimbal-text-dim">
                Original backend by KMS - Martin Dubois, P.Eng
              </div>
            </div>
          </div>

          <div className="flex items-center justify-between">
            <div>
              <div className="text-sm text-gimbal-text">UI Development</div>
              <div className="text-xs text-gimbal-text-dim">
                React-based control interface
              </div>
            </div>
          </div>
        </div>
      </div>

      {/* Links */}
      <div className="flex gap-4">
        <a
          href="#"
          className="flex-1 flex items-center justify-center gap-2 py-3 bg-gimbal-panel border border-gimbal-border rounded-xl text-sm text-gimbal-text hover:border-gimbal-accent transition-colors"
        >
          <Github size={18} />
          GitHub Repository
          <ExternalLink size={14} />
        </a>

        <a
          href="#"
          className="flex-1 flex items-center justify-center gap-2 py-3 bg-gimbal-panel border border-gimbal-border rounded-xl text-sm text-gimbal-text hover:border-gimbal-accent transition-colors"
        >
          <Mail size={18} />
          Contact Support
          <ExternalLink size={14} />
        </a>
      </div>
    </div>
  );
}
