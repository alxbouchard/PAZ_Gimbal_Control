import { useRef, useMemo } from 'react';
import { Canvas, useFrame } from '@react-three/fiber';
import { OrbitControls, PerspectiveCamera, Environment, Html, RoundedBox } from '@react-three/drei';
import * as THREE from 'three';
import { useGimbalStore } from '../../store/gimbalStore';

// Motor housing component - cylindrical motor with details
function MotorHousing({ rotation = [0, 0, 0] as [number, number, number], position = [0, 0, 0] as [number, number, number] }) {
  const motorMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#1a1a1a',
    metalness: 0.3,
    roughness: 0.7,
  }), []);

  const ringMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#e31937', // DJI red accent
    metalness: 0.5,
    roughness: 0.3,
  }), []);

  return (
    <group rotation={rotation} position={position}>
      {/* Main motor body */}
      <mesh material={motorMaterial}>
        <cylinderGeometry args={[0.18, 0.18, 0.15, 32]} />
      </mesh>
      {/* Motor cap */}
      <mesh position={[0, 0.08, 0]} material={motorMaterial}>
        <cylinderGeometry args={[0.14, 0.18, 0.02, 32]} />
      </mesh>
      {/* Red accent ring */}
      <mesh position={[0, -0.06, 0]} material={ringMaterial}>
        <torusGeometry args={[0.16, 0.015, 8, 32]} />
      </mesh>
      {/* Motor shaft hint */}
      <mesh position={[0, -0.08, 0]}>
        <cylinderGeometry args={[0.05, 0.05, 0.02, 16]} />
        <meshStandardMaterial color="#0a0a0a" metalness={0.8} roughness={0.2} />
      </mesh>
    </group>
  );
}

// Arm segment with realistic shape
function ArmSegment({ length = 0.5, width = 0.08 }) {
  const armMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#1e1e1e',
    metalness: 0.2,
    roughness: 0.8,
  }), []);

  return (
    <mesh material={armMaterial}>
      <boxGeometry args={[width, length, width * 0.6]} />
    </mesh>
  );
}

function GimbalModel() {
  const { position } = useGimbalStore();
  const yawRef = useRef<THREE.Group>(null);
  const pitchRef = useRef<THREE.Group>(null);
  const rollRef = useRef<THREE.Group>(null);

  useFrame(() => {
    if (yawRef.current) {
      const currentYaw = yawRef.current.rotation.y;
      const targetYaw = THREE.MathUtils.degToRad(position.yaw);
      let diff = targetYaw - currentYaw;
      while (diff > Math.PI) diff -= Math.PI * 2;
      while (diff < -Math.PI) diff += Math.PI * 2;
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

  // Materials
  const bodyMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#1a1a1a',
    metalness: 0.2,
    roughness: 0.8,
  }), []);

  const gripMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#252525',
    metalness: 0.1,
    roughness: 0.9,
  }), []);

  const screenMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#0a0a0a',
    metalness: 0.9,
    roughness: 0.1,
    emissive: '#1a3a5c',
    emissiveIntensity: 0.3,
  }), []);

  const cameraMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#0f0f0f',
    metalness: 0.7,
    roughness: 0.3,
  }), []);

  const buttonMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#333333',
    metalness: 0.3,
    roughness: 0.6,
  }), []);

  const accentMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#e31937', // DJI red
    metalness: 0.5,
    roughness: 0.4,
  }), []);

  return (
    <group>
      {/* ========== HANDLE/GRIP SECTION ========== */}
      <group position={[0, -1.2, 0]}>
        {/* Main grip body */}
        <RoundedBox args={[0.35, 1.1, 0.3]} radius={0.05} smoothness={4} position={[0, 0, 0]}>
          <primitive object={gripMaterial} attach="material" />
        </RoundedBox>

        {/* Grip texture lines */}
        {[-0.3, -0.15, 0, 0.15, 0.3].map((y, i) => (
          <mesh key={i} position={[0.176, y, 0]}>
            <boxGeometry args={[0.005, 0.08, 0.25]} />
            <meshStandardMaterial color="#1a1a1a" />
          </mesh>
        ))}

        {/* Trigger button */}
        <mesh position={[0, 0.35, 0.12]} rotation={[-0.3, 0, 0]}>
          <boxGeometry args={[0.12, 0.04, 0.08]} />
          <primitive object={buttonMaterial} attach="material" />
        </mesh>

        {/* Front wheel/dial */}
        <mesh position={[0.12, 0.25, 0.15]} rotation={[0, 0, Math.PI / 2]}>
          <cylinderGeometry args={[0.04, 0.04, 0.03, 24]} />
          <primitive object={buttonMaterial} attach="material" />
        </mesh>

        {/* Side buttons */}
        <mesh position={[-0.176, 0.2, 0]}>
          <boxGeometry args={[0.02, 0.08, 0.06]} />
          <primitive object={buttonMaterial} attach="material" />
        </mesh>
        <mesh position={[-0.176, 0.05, 0]}>
          <boxGeometry args={[0.02, 0.08, 0.06]} />
          <primitive object={buttonMaterial} attach="material" />
        </mesh>

        {/* Small screen on handle */}
        <mesh position={[0, 0.42, 0.151]}>
          <planeGeometry args={[0.2, 0.12]} />
          <primitive object={screenMaterial} attach="material" />
        </mesh>

        {/* DJI logo area (red accent) */}
        <mesh position={[0, -0.35, 0.151]}>
          <planeGeometry args={[0.15, 0.04]} />
          <primitive object={accentMaterial} attach="material" />
        </mesh>

        {/* Handle top connector */}
        <mesh position={[0, 0.6, 0]} material={bodyMaterial}>
          <cylinderGeometry args={[0.12, 0.15, 0.1, 24]} />
        </mesh>
      </group>

      {/* ========== YAW MOTOR & ARM ========== */}
      <group ref={yawRef}>
        {/* Yaw motor */}
        <MotorHousing position={[0, -0.55, 0]} rotation={[0, 0, 0]} />

        {/* Yaw arm (vertical) */}
        <group position={[0, -0.1, 0]}>
          <ArmSegment length={0.7} width={0.1} />
        </group>

        {/* Yaw to pitch connector block */}
        <RoundedBox args={[0.25, 0.12, 0.15]} radius={0.02} smoothness={4} position={[0, 0.25, 0]}>
          <primitive object={bodyMaterial} attach="material" />
        </RoundedBox>

        {/* ========== PITCH MOTOR & ARM ========== */}
        <group ref={pitchRef} position={[0, 0.35, 0]}>
          {/* Pitch motor */}
          <MotorHousing position={[0.15, 0, 0]} rotation={[0, 0, Math.PI / 2]} />

          {/* Pitch arm (horizontal going forward) */}
          <group position={[0, 0, 0.35]} rotation={[Math.PI / 2, 0, 0]}>
            <ArmSegment length={0.6} width={0.08} />
          </group>

          {/* Pitch to roll connector */}
          <RoundedBox args={[0.12, 0.1, 0.12]} radius={0.02} smoothness={4} position={[0, 0, 0.65]}>
            <primitive object={bodyMaterial} attach="material" />
          </RoundedBox>

          {/* ========== ROLL MOTOR & CAMERA MOUNT ========== */}
          <group ref={rollRef} position={[0, 0, 0.7]}>
            {/* Roll motor */}
            <MotorHousing position={[0, 0.08, 0]} rotation={[Math.PI / 2, 0, 0]} />

            {/* Camera quick release plate */}
            <mesh position={[0, -0.12, 0]} material={bodyMaterial}>
              <boxGeometry args={[0.5, 0.04, 0.4]} />
            </mesh>

            {/* Quick release lever */}
            <mesh position={[0.22, -0.08, 0]} material={accentMaterial}>
              <boxGeometry args={[0.04, 0.06, 0.08]} />
            </mesh>

            {/* ========== CAMERA ========== */}
            <group position={[0, -0.25, 0]}>
              {/* Camera body */}
              <RoundedBox args={[0.45, 0.28, 0.35]} radius={0.03} smoothness={4}>
                <primitive object={cameraMaterial} attach="material" />
              </RoundedBox>

              {/* Camera grip */}
              <mesh position={[0.26, 0, 0.05]} material={cameraMaterial}>
                <boxGeometry args={[0.08, 0.26, 0.25]} />
              </mesh>

              {/* Lens mount */}
              <mesh position={[0, 0, 0.2]} rotation={[Math.PI / 2, 0, 0]}>
                <cylinderGeometry args={[0.1, 0.12, 0.08, 32]} />
                <meshStandardMaterial color="#1a1a1a" metalness={0.6} roughness={0.3} />
              </mesh>

              {/* Lens body */}
              <mesh position={[0, 0, 0.35]} rotation={[Math.PI / 2, 0, 0]}>
                <cylinderGeometry args={[0.09, 0.09, 0.25, 32]} />
                <meshStandardMaterial color="#0d0d0d" metalness={0.4} roughness={0.5} />
              </mesh>

              {/* Lens front element */}
              <mesh position={[0, 0, 0.48]} rotation={[Math.PI / 2, 0, 0]}>
                <cylinderGeometry args={[0.07, 0.08, 0.02, 32]} />
                <meshStandardMaterial color="#050510" metalness={0.9} roughness={0.1} />
              </mesh>

              {/* Lens glass */}
              <mesh position={[0, 0, 0.495]}>
                <circleGeometry args={[0.065, 32]} />
                <meshStandardMaterial
                  color="#0a0a1a"
                  metalness={1}
                  roughness={0}
                  envMapIntensity={2}
                />
              </mesh>

              {/* Focus ring */}
              <mesh position={[0, 0, 0.28]} rotation={[Math.PI / 2, 0, 0]}>
                <torusGeometry args={[0.095, 0.015, 8, 32]} />
                <meshStandardMaterial color="#252525" metalness={0.2} roughness={0.8} />
              </mesh>

              {/* Camera top (hot shoe, etc) */}
              <mesh position={[0, 0.16, -0.05]} material={cameraMaterial}>
                <boxGeometry args={[0.35, 0.04, 0.2]} />
              </mesh>

              {/* EVF bump */}
              <mesh position={[0, 0.1, -0.2]} material={cameraMaterial}>
                <boxGeometry args={[0.12, 0.1, 0.06]} />
              </mesh>

              {/* Recording indicator */}
              <mesh position={[0.18, 0.12, 0.175]}>
                <sphereGeometry args={[0.015, 16, 16]} />
                <meshStandardMaterial
                  color="#ff0000"
                  emissive="#ff0000"
                  emissiveIntensity={2}
                />
              </mesh>

              {/* Screen on back */}
              <mesh position={[0, -0.02, -0.176]}>
                <planeGeometry args={[0.28, 0.18]} />
                <meshStandardMaterial
                  color="#111122"
                  metalness={0.9}
                  roughness={0.1}
                  emissive="#1a2a3a"
                  emissiveIntensity={0.2}
                />
              </mesh>
            </group>
          </group>
        </group>
      </group>

      {/* Ground reference */}
      <gridHelper args={[6, 12, '#1e1e2e', '#12121a']} position={[0, -1.85, 0]} />

      {/* Ground plane for shadows */}
      <mesh rotation={[-Math.PI / 2, 0, 0]} position={[0, -1.84, 0]} receiveShadow>
        <planeGeometry args={[8, 8]} />
        <shadowMaterial opacity={0.3} />
      </mesh>
    </group>
  );
}

// Normalize angle to -180° to +180° range
function normalizeYaw(yaw: number): number {
  let normalized = yaw % 360;
  if (normalized > 180) normalized -= 360;
  if (normalized < -180) normalized += 360;
  return normalized;
}

function PositionOverlay() {
  const { position } = useGimbalStore();
  const displayYaw = normalizeYaw(position.yaw);

  return (
    <Html position={[2.2, 1.8, 0]} distanceFactor={10}>
      <div className="bg-black/80 backdrop-blur-sm rounded-lg border border-red-900/50 p-3 text-xs font-mono whitespace-nowrap shadow-lg">
        <div className="text-red-500 font-bold mb-2 flex items-center gap-2">
          <span className="w-2 h-2 bg-red-500 rounded-full animate-pulse"></span>
          DJI RONIN RS 3
        </div>
        <div className="space-y-1.5">
          <div className="flex justify-between gap-6">
            <span className="text-gray-500">Pitch</span>
            <span className="text-white">{position.pitch.toFixed(1)}°</span>
          </div>
          <div className="flex justify-between gap-6">
            <span className="text-gray-500">Pan</span>
            <span className="text-white">{displayYaw.toFixed(1)}°</span>
          </div>
          <div className="flex justify-between gap-6">
            <span className="text-gray-500">Roll</span>
            <span className="text-white">{position.roll.toFixed(1)}°</span>
          </div>
        </div>
      </div>
    </Html>
  );
}

export function VirtualGimbal() {
  return (
    <div className="h-full min-h-[400px] bg-gradient-to-b from-[#0a0a0f] to-[#12121a] rounded-xl border border-gray-800 overflow-hidden">
      <Canvas shadows>
        <PerspectiveCamera makeDefault position={[3, 2, 3.5]} fov={45} />
        <OrbitControls
          enablePan={false}
          minDistance={2.5}
          maxDistance={8}
          maxPolarAngle={Math.PI * 0.6}
          minPolarAngle={Math.PI * 0.2}
        />

        {/* Lighting setup */}
        <ambientLight intensity={0.3} />
        <directionalLight
          position={[5, 8, 5]}
          intensity={1.2}
          castShadow
          shadow-mapSize={[2048, 2048]}
          shadow-camera-far={20}
          shadow-camera-left={-5}
          shadow-camera-right={5}
          shadow-camera-top={5}
          shadow-camera-bottom={-5}
        />
        <directionalLight position={[-3, 4, -3]} intensity={0.4} />
        <pointLight position={[0, 3, 2]} intensity={0.5} color="#ffffff" />

        {/* Subtle rim light */}
        <pointLight position={[-2, 0, -2]} intensity={0.3} color="#3b82f6" />

        {/* Environment for realistic reflections */}
        <Environment preset="studio" />

        <GimbalModel />
        <PositionOverlay />
      </Canvas>
    </div>
  );
}
