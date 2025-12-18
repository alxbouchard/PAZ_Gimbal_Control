import { useRef, useMemo } from 'react';
import { Canvas, useFrame } from '@react-three/fiber';
import { OrbitControls, PerspectiveCamera, Environment, Html, RoundedBox } from '@react-three/drei';
import * as THREE from 'three';
import { useGimbalStore } from '../../store/gimbalStore';

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
  const blackMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#1a1a1a',
    metalness: 0.3,
    roughness: 0.7,
  }), []);

  const darkGrayMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#2a2a2a',
    metalness: 0.2,
    roughness: 0.8,
  }), []);

  const motorMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#0f0f0f',
    metalness: 0.4,
    roughness: 0.6,
  }), []);

  const redAccent = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#cc0000',
    metalness: 0.5,
    roughness: 0.4,
  }), []);

  const screenMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#000000',
    metalness: 0.9,
    roughness: 0.1,
    emissive: '#0a1a2a',
    emissiveIntensity: 0.3,
  }), []);

  const lensMaterial = useMemo(() => new THREE.MeshStandardMaterial({
    color: '#1a1a1a',
    metalness: 0.6,
    roughness: 0.4,
  }), []);

  return (
    <group>
      {/* ==================== MINI TRIPOD BASE ==================== */}
      <group position={[0, -1.75, 0]}>
        <mesh material={blackMaterial}>
          <cylinderGeometry args={[0.05, 0.07, 0.05, 24]} />
        </mesh>
        {[0, 120, 240].map((angle, i) => (
          <group key={i} rotation={[0, THREE.MathUtils.degToRad(angle), 0]}>
            <mesh position={[0.1, -0.015, 0]} rotation={[0, 0, -0.2]} material={blackMaterial}>
              <boxGeometry args={[0.12, 0.018, 0.025]} />
            </mesh>
            <mesh position={[0.17, -0.035, 0]} material={darkGrayMaterial}>
              <sphereGeometry args={[0.015, 12, 12]} />
            </mesh>
          </group>
        ))}
      </group>

      {/* ==================== HANDLE / GRIP ==================== */}
      <group position={[0, -1.25, 0]}>
        <RoundedBox args={[0.2, 0.85, 0.2]} radius={0.03} smoothness={4}>
          <primitive object={blackMaterial} attach="material" />
        </RoundedBox>

        {/* Grip texture */}
        <mesh position={[0, -0.1, 0.101]}>
          <boxGeometry args={[0.16, 0.4, 0.006]} />
          <meshStandardMaterial color="#1f1f1f" roughness={0.95} />
        </mesh>

        {/* OLED Screen */}
        <mesh position={[0, 0.28, 0.101]}>
          <planeGeometry args={[0.12, 0.07]} />
          <primitive object={screenMaterial} attach="material" />
        </mesh>

        {/* Focus wheel */}
        <mesh position={[0, 0.08, 0.11]} rotation={[Math.PI / 2, 0, 0]}>
          <cylinderGeometry args={[0.035, 0.035, 0.018, 32]} />
          <meshStandardMaterial color="#2a2a2a" metalness={0.3} roughness={0.7} />
        </mesh>

        {/* Joystick */}
        <mesh position={[0, 0.18, 0.1]}>
          <sphereGeometry args={[0.018, 16, 16]} />
          <meshStandardMaterial color="#1a1a1a" metalness={0.5} roughness={0.5} />
        </mesh>

        {/* Buttons */}
        <mesh position={[0.04, 0.35, 0.1]}>
          <cylinderGeometry args={[0.01, 0.01, 0.006, 16]} />
          <primitive object={redAccent} attach="material" />
        </mesh>
        <mesh position={[-0.04, 0.35, 0.1]}>
          <cylinderGeometry args={[0.01, 0.01, 0.006, 16]} />
          <meshStandardMaterial color="#333333" />
        </mesh>

        {/* Trigger */}
        <mesh position={[0, 0.15, -0.07]} rotation={[0.12, 0, 0]}>
          <boxGeometry args={[0.05, 0.08, 0.02]} />
          <primitive object={darkGrayMaterial} attach="material" />
        </mesh>

        {/* RONIN-S2 label area */}
        <mesh position={[0, 0.38, 0.05]}>
          <boxGeometry args={[0.12, 0.03, 0.1]} />
          <meshStandardMaterial color="#1a1a1a" />
        </mesh>
      </group>

      {/* ==================== YAW AXIS ==================== */}
      <group ref={yawRef}>
        {/* YAW MOTOR - large cylinder above handle */}
        <group position={[0, -0.72, 0]}>
          <mesh material={motorMaterial}>
            <cylinderGeometry args={[0.11, 0.12, 0.1, 32]} />
          </mesh>
          {/* Motor top ring */}
          <mesh position={[0, 0.045, 0]}>
            <torusGeometry args={[0.1, 0.012, 8, 32]} />
            <meshStandardMaterial color="#1a1a1a" metalness={0.5} roughness={0.5} />
          </mesh>
          {/* Motor bottom ring */}
          <mesh position={[0, -0.045, 0]}>
            <torusGeometry args={[0.11, 0.01, 8, 32]} />
            <meshStandardMaterial color="#1a1a1a" metalness={0.4} roughness={0.6} />
          </mesh>
        </group>

        {/* ==================== VERTICAL POST (short section above yaw motor) ==================== */}
        <group position={[0, -0.55, 0]}>
          <mesh material={blackMaterial}>
            <cylinderGeometry args={[0.045, 0.05, 0.22, 24]} />
          </mesh>
        </group>

        {/* ==================== PITCH MOTOR (on the LEFT side, horizontal axis) ==================== */}
        {/* Offset so that roll motor (and camera) ends up centered at X=0 */}
        <group ref={pitchRef} position={[-0.42, 0.1, 0]}>
          {/* Pitch motor housing - horizontal cylinder */}
          <mesh rotation={[0, 0, Math.PI / 2]} material={motorMaterial}>
            <cylinderGeometry args={[0.08, 0.08, 0.12, 32]} />
          </mesh>
          {/* Motor rings */}
          <mesh position={[-0.055, 0, 0]} rotation={[0, 0, Math.PI / 2]}>
            <torusGeometry args={[0.07, 0.008, 8, 32]} />
            <meshStandardMaterial color="#1a1a1a" metalness={0.4} roughness={0.6} />
          </mesh>
          <mesh position={[0.055, 0, 0]} rotation={[0, 0, Math.PI / 2]}>
            <torusGeometry args={[0.07, 0.008, 8, 32]} />
            <meshStandardMaterial color="#1a1a1a" metalness={0.4} roughness={0.6} />
          </mesh>

          {/* ==================== VERTICAL ARM with RONIN text (going DOWN from pitch motor to horizontal arm) ==================== */}
          <group position={[0, -0.15, 0]}>
            <RoundedBox args={[0.07, 0.22, 0.065]} radius={0.012} smoothness={4}>
              <primitive object={blackMaterial} attach="material" />
            </RoundedBox>
            {/* Red accent line */}
            <mesh position={[0.036, 0, 0]}>
              <boxGeometry args={[0.003, 0.18, 0.05]} />
              <primitive object={redAccent} attach="material" />
            </mesh>
            {/* Scale markings */}
            {[-0.06, -0.02, 0.02, 0.06].map((y, i) => (
              <mesh key={i} position={[-0.036, y, 0]}>
                <boxGeometry args={[0.003, 0.006, 0.04]} />
                <meshStandardMaterial color="#444444" />
              </mesh>
            ))}
          </group>

          {/* ==================== HORIZONTAL ARM (going RIGHT from vertical arm to roll motor at X=0) ==================== */}
          <group position={[0.21, -0.26, 0]}>
            <RoundedBox args={[0.48, 0.055, 0.08]} radius={0.01} smoothness={4}>
              <primitive object={blackMaterial} attach="material" />
            </RoundedBox>
            {/* Rail groove on top */}
            <mesh position={[0, 0.028, 0]}>
              <boxGeometry args={[0.44, 0.005, 0.06]} />
              <meshStandardMaterial color="#0a0a0a" />
            </mesh>
            {/* Scale markings */}
            {[-0.18, -0.09, 0, 0.09, 0.18].map((x, i) => (
              <mesh key={i} position={[x, 0.03, 0.03]}>
                <boxGeometry args={[0.006, 0.003, 0.012]} />
                <meshStandardMaterial color="#555555" />
              </mesh>
            ))}
          </group>

          {/* ==================== ROLL MOTOR (positioned so camera is centered on pitch axis Y=0) ==================== */}
          <group position={[0.42, -0.26, 0]}>
            {/* Roll motor - cylinder facing forward (Z axis) */}
            <mesh rotation={[Math.PI / 2, 0, 0]} material={motorMaterial}>
              <cylinderGeometry args={[0.065, 0.065, 0.09, 32]} />
            </mesh>
            {/* Motor ring front */}
            <mesh position={[0, 0, 0.04]} rotation={[Math.PI / 2, 0, 0]}>
              <torusGeometry args={[0.055, 0.005, 8, 32]} />
              <meshStandardMaterial color="#1a1a1a" metalness={0.4} roughness={0.6} />
            </mesh>

            {/* ==================== ROLL GROUP (camera rotates around Z axis from here) ==================== */}
            <group ref={rollRef}>
              {/* ==================== QUICK RELEASE PLATE (CENTERED under roll motor) ==================== */}
              <group position={[0, -0.08, 0]}>
                {/* Main plate */}
                <mesh material={darkGrayMaterial}>
                  <boxGeometry args={[0.35, 0.025, 0.09]} />
                </mesh>
                {/* Side rails */}
                <mesh position={[0, 0, 0.04]}>
                  <boxGeometry args={[0.32, 0.03, 0.01]} />
                  <meshStandardMaterial color="#1a1a1a" />
                </mesh>
                <mesh position={[0, 0, -0.04]}>
                  <boxGeometry args={[0.32, 0.03, 0.01]} />
                  <meshStandardMaterial color="#1a1a1a" />
                </mesh>
                {/* Adjustment knobs */}
                <mesh position={[0.14, 0.02, 0]}>
                  <cylinderGeometry args={[0.012, 0.012, 0.015, 16]} />
                  <meshStandardMaterial color="#333333" />
                </mesh>
                <mesh position={[-0.14, 0.02, 0]}>
                  <cylinderGeometry args={[0.01, 0.01, 0.012, 16]} />
                  <meshStandardMaterial color="#cc0000" />
                </mesh>
              </group>

              {/* ==================== CAMERA (CENTERED on roll axis, facing FORWARD / +Z) ==================== */}
              {/* Adjust Y so camera center aligns with pitch motor axis (Y=0 relative to pitch) */}
              <group position={[0, 0.26, 0]}>
              {/* Camera body - DSLR style */}
              <RoundedBox args={[0.38, 0.28, 0.22]} radius={0.015} smoothness={4}>
                <meshStandardMaterial color="#3a3a3a" metalness={0.2} roughness={0.8} transparent opacity={0.85} />
              </RoundedBox>

              {/* Camera grip (right side) */}
              <mesh position={[0.16, -0.02, 0.06]}>
                <boxGeometry args={[0.06, 0.26, 0.1]} />
                <meshStandardMaterial color="#3a3a3a" metalness={0.2} roughness={0.8} transparent opacity={0.85} />
              </mesh>

              {/* Top prism/pentaprism housing */}
              <mesh position={[0, 0.18, -0.02]}>
                <boxGeometry args={[0.14, 0.08, 0.12]} />
                <meshStandardMaterial color="#3a3a3a" metalness={0.2} roughness={0.8} transparent opacity={0.85} />
              </mesh>

              {/* Hot shoe */}
              <mesh position={[0, 0.23, -0.02]}>
                <boxGeometry args={[0.06, 0.012, 0.04]} />
                <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.4} />
              </mesh>

              {/* Mode dial */}
              <mesh position={[0.1, 0.16, 0.04]}>
                <cylinderGeometry args={[0.025, 0.025, 0.018, 24]} />
                <meshStandardMaterial color="#2a2a2a" metalness={0.5} roughness={0.5} />
              </mesh>

              {/* ==================== LENS (pointing FORWARD / +Z) ==================== */}
              {/* Lens mount */}
              <mesh position={[0, 0, 0.12]} rotation={[Math.PI / 2, 0, 0]}>
                <cylinderGeometry args={[0.08, 0.085, 0.025, 32]} />
                <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.4} />
              </mesh>

              {/* Lens barrel */}
              <mesh position={[0, 0, 0.2]} rotation={[Math.PI / 2, 0, 0]}>
                <cylinderGeometry args={[0.075, 0.08, 0.12, 32]} />
                <primitive object={lensMaterial} attach="material" />
              </mesh>

              {/* Zoom ring */}
              <mesh position={[0, 0, 0.3]} rotation={[Math.PI / 2, 0, 0]}>
                <cylinderGeometry args={[0.082, 0.082, 0.1, 32]} />
                <meshStandardMaterial color="#1a1a1a" metalness={0.15} roughness={0.9} />
              </mesh>

              {/* Zoom ring texture */}
              {[0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330].map((angle, i) => (
                <mesh key={i} position={[
                  Math.cos(THREE.MathUtils.degToRad(angle)) * 0.084,
                  Math.sin(THREE.MathUtils.degToRad(angle)) * 0.084,
                  0.3
                ]} rotation={[0, 0, THREE.MathUtils.degToRad(angle)]}>
                  <boxGeometry args={[0.008, 0.004, 0.08]} />
                  <meshStandardMaterial color="#0f0f0f" />
                </mesh>
              ))}

              {/* Focus ring */}
              <mesh position={[0, 0, 0.4]} rotation={[Math.PI / 2, 0, 0]}>
                <cylinderGeometry args={[0.072, 0.078, 0.08, 32]} />
                <meshStandardMaterial color="#1f1f1f" metalness={0.2} roughness={0.85} />
              </mesh>

              {/* Lens front */}
              <mesh position={[0, 0, 0.48]} rotation={[Math.PI / 2, 0, 0]}>
                <cylinderGeometry args={[0.065, 0.07, 0.08, 32]} />
                <meshStandardMaterial color="#151515" metalness={0.3} roughness={0.7} />
              </mesh>

              {/* Front element (glass) */}
              <mesh position={[0, 0, 0.525]}>
                <circleGeometry args={[0.055, 32]} />
                <meshStandardMaterial
                  color="#0a1520"
                  metalness={1}
                  roughness={0}
                  envMapIntensity={2}
                />
              </mesh>

              {/* Inner lens reflection ring */}
              <mesh position={[0, 0, 0.52]} rotation={[Math.PI / 2, 0, 0]}>
                <torusGeometry args={[0.045, 0.008, 8, 32]} />
                <meshStandardMaterial color="#0a0a0a" metalness={0.8} roughness={0.2} />
              </mesh>
            </group>
            </group>
          </group>
        </group>
      </group>

      {/* Ground */}
      <mesh rotation={[-Math.PI / 2, 0, 0]} position={[0, -1.82, 0]} receiveShadow>
        <circleGeometry args={[3, 64]} />
        <meshStandardMaterial color="#0a0a0a" metalness={0.1} roughness={0.9} />
      </mesh>

      <mesh rotation={[-Math.PI / 2, 0, 0]} position={[0, -1.81, 0]}>
        <circleGeometry args={[0.5, 32]} />
        <meshBasicMaterial color="#000000" opacity={0.3} transparent />
      </mesh>
    </group>
  );
}

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
    <Html position={[2, 1.5, 0]} distanceFactor={10}>
      <div className="bg-black/90 backdrop-blur-sm rounded-lg border border-red-900/50 p-3 text-xs font-mono whitespace-nowrap shadow-xl">
        <div className="text-red-500 font-bold mb-2 flex items-center gap-2">
          <span className="w-2 h-2 bg-red-500 rounded-full animate-pulse"></span>
          DJI RONIN RS 3
        </div>
        <div className="space-y-1.5">
          <div className="flex justify-between gap-6">
            <span className="text-gray-500">Tilt</span>
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
        <PerspectiveCamera makeDefault position={[2, 1, 2.5]} fov={45} />
        <OrbitControls
          enablePan={false}
          minDistance={2}
          maxDistance={6}
          maxPolarAngle={Math.PI * 0.65}
          minPolarAngle={Math.PI * 0.15}
          target={[0, -0.3, 0]}
        />

        {/* Lighting */}
        <ambientLight intensity={0.4} />
        <directionalLight
          position={[5, 8, 5]}
          intensity={1}
          castShadow
          shadow-mapSize={[2048, 2048]}
        />
        <directionalLight position={[-3, 4, -3]} intensity={0.3} />
        <pointLight position={[0, 2, 3]} intensity={0.4} color="#ffffff" />
        <pointLight position={[-2, 0, -1]} intensity={0.2} color="#4a90d9" />

        <Environment preset="studio" />

        <GimbalModel />
        <PositionOverlay />
      </Canvas>
    </div>
  );
}
