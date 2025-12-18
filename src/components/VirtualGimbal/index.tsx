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
    color: '#252525',
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
    color: '#0a0a0a',
    metalness: 0.6,
    roughness: 0.4,
  }), []);

  return (
    <group>
      {/* ==================== MINI TRIPOD BASE ==================== */}
      <group position={[0, -1.65, 0]}>
        {/* Tripod center */}
        <mesh material={blackMaterial}>
          <cylinderGeometry args={[0.08, 0.1, 0.08, 24]} />
        </mesh>
        {/* Tripod legs */}
        {[0, 120, 240].map((angle, i) => (
          <group key={i} rotation={[0, THREE.MathUtils.degToRad(angle), 0]}>
            <mesh position={[0.15, -0.02, 0]} rotation={[0, 0, -0.3]} material={blackMaterial}>
              <boxGeometry args={[0.18, 0.025, 0.04]} />
            </mesh>
            {/* Leg foot */}
            <mesh position={[0.26, -0.04, 0]} material={darkGrayMaterial}>
              <sphereGeometry args={[0.025, 16, 16]} />
            </mesh>
          </group>
        ))}
      </group>

      {/* ==================== HANDLE / GRIP ==================== */}
      <group position={[0, -1.1, 0]}>
        {/* Main grip body */}
        <RoundedBox args={[0.28, 0.9, 0.26]} radius={0.04} smoothness={4}>
          <primitive object={blackMaterial} attach="material" />
        </RoundedBox>

        {/* Grip rubberized texture (front) */}
        <mesh position={[0, -0.1, 0.131]}>
          <boxGeometry args={[0.24, 0.5, 0.01]} />
          <meshStandardMaterial color="#1f1f1f" roughness={0.95} />
        </mesh>

        {/* OLED Screen */}
        <mesh position={[0, 0.28, 0.131]}>
          <planeGeometry args={[0.18, 0.1]} />
          <primitive object={screenMaterial} attach="material" />
        </mesh>

        {/* Focus/Zoom wheel (front) */}
        <mesh position={[0, 0.05, 0.14]} rotation={[Math.PI / 2, 0, 0]}>
          <cylinderGeometry args={[0.05, 0.05, 0.025, 32]} />
          <meshStandardMaterial color="#2a2a2a" metalness={0.3} roughness={0.7} />
        </mesh>
        {/* Wheel grip lines */}
        <mesh position={[0, 0.05, 0.155]} rotation={[Math.PI / 2, 0, 0]}>
          <torusGeometry args={[0.045, 0.004, 8, 24]} />
          <meshStandardMaterial color="#1a1a1a" />
        </mesh>

        {/* Joystick */}
        <mesh position={[0, 0.18, 0.13]}>
          <sphereGeometry args={[0.025, 16, 16]} />
          <meshStandardMaterial color="#1a1a1a" metalness={0.5} roughness={0.5} />
        </mesh>

        {/* Mode button (M) */}
        <mesh position={[-0.06, 0.35, 0.13]}>
          <cylinderGeometry args={[0.015, 0.015, 0.01, 16]} />
          <meshStandardMaterial color="#333333" />
        </mesh>

        {/* Record button (red) */}
        <mesh position={[0.06, 0.35, 0.13]}>
          <cylinderGeometry args={[0.015, 0.015, 0.01, 16]} />
          <primitive object={redAccent} attach="material" />
        </mesh>

        {/* Trigger (back) */}
        <mesh position={[0, 0.15, -0.1]} rotation={[0.2, 0, 0]}>
          <boxGeometry args={[0.08, 0.12, 0.03]} />
          <primitive object={darkGrayMaterial} attach="material" />
        </mesh>

        {/* DJI Logo area */}
        <mesh position={[0.1, 0.32, 0.08]}>
          <boxGeometry args={[0.04, 0.025, 0.02]} />
          <meshStandardMaterial color="#cccccc" />
        </mesh>

        {/* Side buttons (left) */}
        <mesh position={[-0.141, 0.2, 0]}>
          <boxGeometry args={[0.015, 0.06, 0.04]} />
          <primitive object={darkGrayMaterial} attach="material" />
        </mesh>
        <mesh position={[-0.141, 0.1, 0]}>
          <boxGeometry args={[0.015, 0.06, 0.04]} />
          <primitive object={darkGrayMaterial} attach="material" />
        </mesh>
      </group>

      {/* ==================== YAW MOTOR (base of arm) ==================== */}
      <group ref={yawRef}>
        <group position={[0, -0.55, 0]}>
          {/* Yaw motor housing */}
          <mesh material={motorMaterial}>
            <cylinderGeometry args={[0.12, 0.14, 0.1, 32]} />
          </mesh>
          {/* Motor ring detail */}
          <mesh position={[0, 0.04, 0]}>
            <torusGeometry args={[0.11, 0.015, 8, 32]} />
            <meshStandardMaterial color="#1a1a1a" metalness={0.5} roughness={0.5} />
          </mesh>
        </group>

        {/* ==================== MAIN ARM (L-shaped, angled) ==================== */}
        {/* Vertical section of arm */}
        <group position={[0, -0.25, 0]}>
          <RoundedBox args={[0.12, 0.5, 0.1]} radius={0.02} smoothness={4}>
            <primitive object={blackMaterial} attach="material" />
          </RoundedBox>
        </group>

        {/* Angled/curved section going to pitch motor */}
        <group position={[0.15, 0.15, 0]} rotation={[0, 0, -0.4]}>
          <RoundedBox args={[0.4, 0.1, 0.09]} radius={0.02} smoothness={4}>
            <primitive object={blackMaterial} attach="material" />
          </RoundedBox>

          {/* RONIN text (red) on arm */}
          <mesh position={[0, 0.051, 0]} rotation={[-Math.PI / 2, 0, 0]}>
            <planeGeometry args={[0.3, 0.06]} />
            <primitive object={redAccent} attach="material" />
          </mesh>
        </group>

        {/* ==================== PITCH MOTOR ==================== */}
        <group ref={pitchRef} position={[0.38, 0.28, 0]}>
          {/* Pitch motor housing */}
          <mesh rotation={[0, 0, Math.PI / 2]} material={motorMaterial}>
            <cylinderGeometry args={[0.1, 0.1, 0.12, 32]} />
          </mesh>

          {/* Pitch arm going forward to roll */}
          <group position={[0, 0, 0.25]} rotation={[0, 0, 0]}>
            <RoundedBox args={[0.08, 0.08, 0.4]} radius={0.015} smoothness={4}>
              <primitive object={blackMaterial} attach="material" />
            </RoundedBox>
          </group>

          {/* ==================== ROLL MOTOR & CAMERA PLATE ==================== */}
          <group ref={rollRef} position={[0, 0, 0.5]}>
            {/* Roll motor (horizontal orientation like in image) */}
            <mesh rotation={[Math.PI / 2, 0, 0]} material={motorMaterial}>
              <cylinderGeometry args={[0.09, 0.09, 0.1, 32]} />
            </mesh>

            {/* Camera mounting plate system */}
            <group position={[0, -0.12, 0]}>
              {/* Upper plate rail */}
              <mesh material={blackMaterial}>
                <boxGeometry args={[0.45, 0.03, 0.12]} />
              </mesh>

              {/* Balance adjustment knobs */}
              <mesh position={[0.2, 0.02, 0.05]} rotation={[Math.PI / 2, 0, 0]}>
                <cylinderGeometry args={[0.025, 0.025, 0.03, 16]} />
                <meshStandardMaterial color="#333333" metalness={0.4} roughness={0.6} />
              </mesh>
              <mesh position={[-0.2, 0.02, 0.05]} rotation={[Math.PI / 2, 0, 0]}>
                <cylinderGeometry args={[0.025, 0.025, 0.03, 16]} />
                <meshStandardMaterial color="#333333" metalness={0.4} roughness={0.6} />
              </mesh>

              {/* Lower plate / quick release */}
              <mesh position={[0, -0.04, 0]} material={darkGrayMaterial}>
                <boxGeometry args={[0.5, 0.025, 0.35]} />
              </mesh>

              {/* Balance scale markings */}
              {[-0.18, -0.09, 0, 0.09, 0.18].map((x, i) => (
                <mesh key={i} position={[x, -0.02, 0.13]}>
                  <boxGeometry args={[0.008, 0.01, 0.02]} />
                  <meshStandardMaterial color="#444444" />
                </mesh>
              ))}

              {/* ==================== CAMERA ==================== */}
              <group position={[0, -0.2, 0]}>
                {/* Camera body */}
                <RoundedBox args={[0.44, 0.28, 0.34]} radius={0.02} smoothness={4}>
                  <primitive object={blackMaterial} attach="material" />
                </RoundedBox>

                {/* Camera grip (right side) */}
                <mesh position={[0.26, 0, 0.02]} material={blackMaterial}>
                  <boxGeometry args={[0.08, 0.3, 0.26]} />
                </mesh>

                {/* Top plate with controls */}
                <mesh position={[0, 0.15, -0.02]} material={blackMaterial}>
                  <boxGeometry args={[0.38, 0.02, 0.28]} />
                </mesh>

                {/* Mode dial */}
                <mesh position={[0.12, 0.17, -0.06]} rotation={[0, 0, 0]}>
                  <cylinderGeometry args={[0.035, 0.035, 0.025, 24]} />
                  <meshStandardMaterial color="#1a1a1a" metalness={0.6} roughness={0.4} />
                </mesh>

                {/* Hot shoe */}
                <mesh position={[0, 0.165, -0.02]}>
                  <boxGeometry args={[0.08, 0.015, 0.06]} />
                  <meshStandardMaterial color="#1f1f1f" metalness={0.7} roughness={0.3} />
                </mesh>

                {/* EVF hump */}
                <group position={[0, 0.08, -0.2]}>
                  <mesh material={blackMaterial}>
                    <boxGeometry args={[0.12, 0.1, 0.06]} />
                  </mesh>
                  {/* EVF eyepiece */}
                  <mesh position={[0, 0, -0.035]} rotation={[Math.PI / 2, 0, 0]}>
                    <cylinderGeometry args={[0.035, 0.04, 0.02, 24]} />
                    <meshStandardMaterial color="#0a0a0a" metalness={0.8} roughness={0.2} />
                  </mesh>
                </group>

                {/* Rear LCD screen */}
                <group position={[0, -0.02, -0.175]}>
                  <mesh>
                    <boxGeometry args={[0.32, 0.2, 0.015]} />
                    <meshStandardMaterial color="#0a0a0a" metalness={0.8} roughness={0.2} />
                  </mesh>
                  <mesh position={[0, 0, -0.008]}>
                    <planeGeometry args={[0.3, 0.18]} />
                    <meshStandardMaterial
                      color="#050510"
                      metalness={0.95}
                      roughness={0.05}
                      emissive="#0a1520"
                      emissiveIntensity={0.1}
                    />
                  </mesh>
                </group>

                {/* ==================== LENS ==================== */}
                {/* Lens mount */}
                <mesh position={[0, 0, 0.18]} rotation={[Math.PI / 2, 0, 0]}>
                  <cylinderGeometry args={[0.1, 0.11, 0.04, 32]} />
                  <meshStandardMaterial color="#1a1a1a" metalness={0.7} roughness={0.3} />
                </mesh>

                {/* Lens body - wide zoom lens */}
                <mesh position={[0, 0, 0.28]} rotation={[Math.PI / 2, 0, 0]}>
                  <cylinderGeometry args={[0.095, 0.1, 0.16, 32]} />
                  <primitive object={lensMaterial} attach="material" />
                </mesh>

                {/* Zoom ring */}
                <mesh position={[0, 0, 0.4]} rotation={[Math.PI / 2, 0, 0]}>
                  <cylinderGeometry args={[0.1, 0.1, 0.12, 32]} />
                  <meshStandardMaterial color="#151515" metalness={0.2} roughness={0.85} />
                </mesh>

                {/* Zoom ring texture */}
                {[0, 60, 120, 180, 240, 300].map((angle, i) => (
                  <mesh key={i} position={[
                    Math.cos(THREE.MathUtils.degToRad(angle)) * 0.102,
                    Math.sin(THREE.MathUtils.degToRad(angle)) * 0.102,
                    0.4
                  ]} rotation={[0, 0, THREE.MathUtils.degToRad(angle)]}>
                    <boxGeometry args={[0.008, 0.015, 0.1]} />
                    <meshStandardMaterial color="#1a1a1a" />
                  </mesh>
                ))}

                {/* Focus ring */}
                <mesh position={[0, 0, 0.52]} rotation={[Math.PI / 2, 0, 0]}>
                  <cylinderGeometry args={[0.088, 0.095, 0.08, 32]} />
                  <meshStandardMaterial color="#1a1a1a" metalness={0.15} roughness={0.9} />
                </mesh>

                {/* Lens front section */}
                <mesh position={[0, 0, 0.6]} rotation={[Math.PI / 2, 0, 0]}>
                  <cylinderGeometry args={[0.08, 0.085, 0.08, 32]} />
                  <meshStandardMaterial color="#0d0d0d" metalness={0.4} roughness={0.6} />
                </mesh>

                {/* Front element / glass */}
                <mesh position={[0, 0, 0.65]}>
                  <circleGeometry args={[0.07, 32]} />
                  <meshStandardMaterial
                    color="#020210"
                    metalness={1}
                    roughness={0}
                    envMapIntensity={2}
                  />
                </mesh>

                {/* Lens hood mount ring */}
                <mesh position={[0, 0, 0.58]} rotation={[Math.PI / 2, 0, 0]}>
                  <torusGeometry args={[0.082, 0.005, 8, 32]} />
                  <meshStandardMaterial color="#1a1a1a" metalness={0.3} roughness={0.7} />
                </mesh>
              </group>
            </group>
          </group>
        </group>
      </group>

      {/* Ground */}
      <mesh rotation={[-Math.PI / 2, 0, 0]} position={[0, -1.72, 0]} receiveShadow>
        <circleGeometry args={[3, 64]} />
        <meshStandardMaterial color="#0a0a0a" metalness={0.1} roughness={0.9} />
      </mesh>

      {/* Subtle ground shadow */}
      <mesh rotation={[-Math.PI / 2, 0, 0]} position={[0, -1.71, 0]}>
        <circleGeometry args={[0.8, 32]} />
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
        <PerspectiveCamera makeDefault position={[2.5, 1.5, 2.5]} fov={45} />
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
