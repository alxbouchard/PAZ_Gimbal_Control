# PAZ Gimbal Control - Future Ideas

## Auto-Tracking Mode (Priority: High)

### Current State
- `Track_Switch()` et `Track_Speed_Set()` utilisent le **DJI ActiveTrack** intégré au gimbal
- Le gimbal utilise sa propre caméra/capteur pour suivre un sujet
- Déjà accessible via le bouton "Track" dans l'UI

### Future: External Auto-Tracking System

Pour un tracking indépendant du DJI (ex: tracking basé sur feed vidéo externe):

1. **Architecture proposée**
   - Source vidéo: Feed ATEM ou caméra dédiée
   - Détection: AI/Computer Vision (pose detection, face tracking)
   - Contrôle: Envoyer commandes position au gimbal via notre API

2. **Modes possibles**
   - **Follow Mode**: Suivi d'un sujet détecté (visage, corps, costume spécifique)
   - **Zone Mode**: Tracking dans une zone prédéfinie de la scène
   - **Preset Mode**: Alternance entre positions prédéfinies avec smooth transitions

3. **UI Concept**
   - Toggle "Auto Track" par gimbal dans le Gimbal Switcher
   - Indicateur visuel quand un gimbal est en mode auto
   - Override manuel instantané quand l'opérateur prend le contrôle
   - Configuration des zones/sujets à tracker

4. **Technical Considerations**
   - Latence de tracking vs mouvement manuel
   - Calibration caméra → coordonnées gimbal
   - Transition smooth entre auto et manuel
   - GPU requis pour détection en temps réel

---

## Other Ideas

### Preset Positions
- Sauvegarder des positions de caméra (wide shot, close-up, etc.)
- Recall rapide via numéros (Shift+1, Shift+2...)
- Transitions smooth entre presets

### Recording Integration
- Bouton REC pour contrôler l'enregistrement ATEM
- Tally light status dans l'UI

### Multi-View
- Voir toutes les caméras en miniature
- Click pour prendre le contrôle

### Gamepad per Camera
- Assigner un gamepad spécifique à un gimbal spécifique
- Permettre contrôle simultané par plusieurs opérateurs avec plusieurs gamepads

---

*Last updated: 2025-01-11*
