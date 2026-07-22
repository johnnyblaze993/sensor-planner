# Geospatial and radar vocabulary

- **Latitude/longitude:** angular position north/south and east/west.
- **Altitude:** height relative to a defined vertical reference. It is not
  automatically the same as terrain elevation or height above ground.
- **Degrees/radians:** human geographic inputs commonly use degrees; C++ trig
  functions use radians.
- **Great-circle distance:** shortest surface path on an ideal sphere.
- **Haversine:** numerically convenient spherical distance formula used here.
- **Vincenty:** algorithms using an ellipsoid; usually more accurate and complex.
- **CRS:** coordinate reference system defining how numbers map to Earth.
- **DTED:** Digital Terrain Elevation Data, a terrain-elevation format.
- **Line of sight:** an unobstructed geometric path between two positions.
- **Terrain masking:** terrain blocks a sensor's view even when pure curvature
  suggests visibility.
- **Effective Earth radius:** simplified propagation model that adjusts the
  geometric horizon; this project uses only a common approximation.
- **Radar range:** distance over which a radar may perform a specified function.
- **Probability of detection/acquisition:** likelihood of detecting or beginning
  to track a target under stated conditions.
- **SNR:** signal-to-noise ratio, desired signal power relative to noise.
- **Radar cross section:** a target-dependent measure of reflected radar energy.
- **Phased array:** antenna whose element phases steer a beam electronically.
- **Search plan:** allocation of sensor attention across regions and time.
- **Tracking:** estimating a target's changing state from observations.
- **Threat model:** systematic description of possible hazards/adversaries and
  relevant capabilities; in cybersecurity the same term concerns attackers.

Vocabulary is context-dependent. Ask which units, reference frames, assumptions,
and probability definitions a team uses before interpreting a value.
