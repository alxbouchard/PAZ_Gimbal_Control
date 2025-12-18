import { useRef, useMemo } from 'react';
import { Canvas, useFrame } from '@react-three/fiber';
import { OrbitControls, PerspectiveCamera, Environment, Html } from '@react-three/drei';
import * as THREE from 'three';
import { useGimbalStore } from '../../store/gimbalStore';

function GimbalModel() {
  const { position } = useGimbalStore();
  const yawRef = useRef<THREE.Group>(null);
  const pitchRef = useRef<THREE.Group>(null);
  const rollRef = useRef<THREE.Group>(null);

  // Smoothly interpolate to target position
  // Uses shortest path for yaw to handle continuous rotation properly
  useFrame(() => {
    if (yawRef.current) {
      const currentYaw = yawRef.current.rotation.y;
      const targetYaw = THREE.MathUtils.degToRad(position.yaw);

      // Calculate shortest path difference (handles wrap-around)
      let diff = targetYaw - currentYaw;
      // Normalize to [-PI, PI] for shortest path
      while (diff > Math.PI) diff -= Math.PI * 2;
      while (diff < -Math.PI) diff += Math.PI * 2;

      // Apply lerp on the difference, not the absolute value
      yawRef.current.rotation.y = currentYaw + diff * 0.1;
    }
    if (pitchRef.current) {
      pitchRef.current.rotation.x = THREE.MathUtils.lerp(
        pitchRef.current.rotation.x,
        THREE.MathUtils.degToRad(position.pitch),
        0.1
      );
    }
    if (rollRef.current) {
      rollRef.current.rotation.z = THREE.MathUtils.lerp(
        rollRef.current.rotation.z,
        THREE.MathUtils.degToRad(position.roll),
        0.1
      );
    }
  });

  const ringMaterial = useMemo(
    () =>
      new THREE.MeshStandardMaterial({
        color: '#3b82f6',
        metalness: 0.8,
        roughness: 0.2,
      }),
    []
  );

  const bodyMaterial = useMemo(
    () =>
      new THREE.MeshStandardMaterial({
        color: '#1e1e2e',
        metalness: 0.6,
        roughness: 0.4,
      }),
    []
  );

  const cameraMaterial = useMemo(
    () =>
      new THREE.MeshStandardMaterial({
        color: '#0a0a0f',
        metalness: 0.9,
        roughness: 0.1,
      }),
    []
  );

  return (
    <group>
      {/* Base */}
      <mesh position={[0, -1.5, 0]} material={bodyMaterial}>
        <cylinderGeometry args={[0.8, 1, 0.3, 32]} />
      </mesh>

      {/* Yaw ring (outer) */}
      <group ref={yawRef}>
        <mesh material={ringMaterial}>
          <torusGeometry args={[1.2, 0.08, 16, 64]} />
        </mesh>

        {/* Vertical arm */}
        <mesh position={[0, 0.5, 0]} material={bodyMaterial}>
          <boxGeometry args={[0.15, 1, 0.15]} />
        </mesh>

        {/* Pitch ring */}
        <group ref={pitchRef} position={[0, 1, 0]}>
          <mesh rotation={[Math.PI / 2, 0, 0]} material={ringMaterial}>
            <torusGeometry args={[0.9, 0.06, 16, 64]} />
          </mesh>

          {/* Roll group */}
          <group ref={rollRef}>
            {/* Roll ring */}
            <mesh rotation={[0, Math.PI / 2, 0]} material={ringMaterial}>
              <torusGeometry args={[0.6, 0.05, 16, 64]} />
            </mesh>

            {/* Camera body */}
            <mesh position={[0, 0, 0]} material={cameraMaterial}>
              <boxGeometry args={[0.5, 0.3, 0.7]} />
            </mesh>

            {/* Camera lens */}
            <mesh position={[0, 0, 0.45]} rotation={[Math.PI / 2, 0, 0]} material={cameraMaterial}>
              <cylinderGeometry args={[0.15, 0.18, 0.2, 32]} />
            </mesh>

            {/* Lens glass */}
            <mesh position={[0, 0, 0.56]} rotation={[Math.PI / 2, 0, 0]}>
              <circleGeometry args={[0.12, 32]} />
              <meshStandardMaterial
                color="#1a1a2e"
                metalness={1}
                roughness={0}
              />
            </mesh>

            {/* Camera indicator light */}
            <mesh position={[0.2, 0.1, 0.35]}>
              <sphereGeometry args={[0.02, 16, 16]} />
              <meshStandardMaterial
                color="#22c55e"
                emissive="#22c55e"
                emissiveIntensity={2}
              />
            </mesh>
          </group>
        </group>
      </group>

      {/* Ground grid */}
      <gridHelper
        args={[10, 20, '#1e1e2e', '#12121a']}
        position={[0, -1.7, 0]}
      />
    </group>
  );
}

// Normalize angle to -180° to +180° range (like DJI Ronin RS display)
function normalizeYaw(yaw: number): number {
  let normalized = yaw % 360;
  if (normalized > 180) normalized -= 360;
  if (normalized < -180) normalized += 360;
  return normalized;
}

function PositionOverlay() {
  const { position } = useGimbalStore();

  // Normalize yaw for display (like real DJI Ronin RS)
  const displayYaw = normalizeYaw(position.yaw);

  return (
    <Html position={[2.5, 2, 0]} distanceFactor={10}>
      <div className="bg-gimbal-panel/90 backdrop-blur-sm rounded-lg border border-gimbal-border p-3 text-xs font-mono whitespace-nowrap">
        <div className="text-gimbal-text-dim mb-2">Position</div>
        <div className="space-y-1">
          <div className="flex justify-between gap-4">
            <span className="text-gimbal-text-dim">Pitch:</span>
            <span className="text-gimbal-accent">{position.pitch.toFixed(1)}°</span>
          </div>
          <div className="flex justify-between gap-4">
            <span className="text-gimbal-text-dim">Yaw:</span>
            <span className="text-gimbal-accent">{displayYaw.toFixed(1)}°</span>
          </div>
          <div className="flex justify-between gap-4">
            <span className="text-gimbal-text-dim">Roll:</span>
            <span className="text-gimbal-accent">{position.roll.toFixed(1)}°</span>
          </div>
        </div>
      </div>
    </Html>
  );
}

export function VirtualGimbal() {
  return (
    <div className="h-full min-h-[400px] bg-gimbal-panel rounded-xl border border-gimbal-border overflow-hidden">
      <Canvas>
        <PerspectiveCamera makeDefault position={[4, 3, 4]} fov={50} />
        <OrbitControls
          enablePan={false}
          minDistance={3}
          maxDistance={10}
          maxPolarAngle={Math.PI / 2}
        />

        {/* Lighting */}
        <ambientLight intensity={0.4} />
        <directionalLight position={[5, 5, 5]} intensity={1} castShadow />
        <directionalLight position={[-5, 3, -5]} intensity={0.3} />
        <pointLight position={[0, 5, 0]} intensity={0.5} />

        {/* Environment for reflections */}
        <Environment preset="city" />

        {/* Gimbal model */}
        <GimbalModel />

        {/* Position overlay */}
        <PositionOverlay />
      </Canvas>
    </div>
  );
}
