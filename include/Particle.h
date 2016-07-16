#ifndef PARTICLE
#define PARTICLE

#include <eigen3/Eigen/Core>

#include <ngl/Vec3.h>
#include <ngl/Mat3.h>

//------------------------------------------------------------------------------------------------------------------------------------------------------
/// @file Particle.h
/// @brief Particle structure containing data specific to one particle
/// @author Ina M. Sorensen
/// @version 1.0
/// @date 25.06.16
///
/// @todo
//------------------------------------------------------------------------------------------------------------------------------------------------------

enum Phase
{
  Solid,
  Liquid
};

class Emitter;

class Particle
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Particle constructor
  //----------------------------------------------------------------------------------------------------------------------
  Particle(Eigen::Vector3f _position, float _mass, float _temperature, bool _isSolid, float _latentHeat, Emitter* _emitter);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Particle destructor
  //----------------------------------------------------------------------------------------------------------------------
  ~Particle();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set Lame coefficients
  //----------------------------------------------------------------------------------------------------------------------
  void setLameCoefficients(float _lameMuConstant, float _lameLambdaConstant, float _hardnessCoefficient);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get particle position
  //----------------------------------------------------------------------------------------------------------------------
  inline Eigen::Vector3f getPosition(){return m_position;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get particle data for grid cell face
  //----------------------------------------------------------------------------------------------------------------------
  void getParticleData_CellFace(float &o_mass, Eigen::Vector3f &o_velocity, Phase &o_phase);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get particle data for grid cell centre
  //----------------------------------------------------------------------------------------------------------------------
  void getParticleData_CellCentre(float &o_mass, float &o_detDeformGrad, float &o_detDeformGradElast, Phase &o_phase, float &o_temp, float &o_lameLambdaInverse);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get particle data for grid cell centre
  //----------------------------------------------------------------------------------------------------------------------
  inline void addParticleDensity(float _densityIncrease){m_initialDensity+=_densityIncrease;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Calculate initial volume
  //----------------------------------------------------------------------------------------------------------------------
  inline void calcInitialVolume(){m_initialVolume=m_mass/m_initialDensity;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get particle volume
  //----------------------------------------------------------------------------------------------------------------------
  inline float getVolume(){return m_initialVolume;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get Lame Mu coefficient
  //----------------------------------------------------------------------------------------------------------------------
  inline float getLameMu(){return m_lameMu;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get dimension, used to calculate deviatoric forces and velocity
  //----------------------------------------------------------------------------------------------------------------------
  inline float getDimension(){return ((float)m_dimension);}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get determinant of elastic deformation gradient det(FE)
  //----------------------------------------------------------------------------------------------------------------------
  inline float getDetDeformationElastic(){return m_detDeformGradElastic;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get elastic deformation gradient, FE
  //----------------------------------------------------------------------------------------------------------------------
  inline Eigen::Matrix3f getDeformationElastic(){return m_deformationElastic;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get J^{-1/d}F
  //----------------------------------------------------------------------------------------------------------------------
  inline Eigen::Matrix3f getDeformationElastic_Deviatoric(){return m_deformationElastic_Deviatoric;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get R from polar decomposition of J^{-1/d}F
  //----------------------------------------------------------------------------------------------------------------------
  inline Eigen::Matrix3f getR_deformationElastic_Deviatoric(){return m_R_deformationElastic_Deviatoric;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get S from polar decomposition of J^{-1/d}F
  //----------------------------------------------------------------------------------------------------------------------
  inline Eigen::Matrix3f getS_deformationElastic_Deviatoric(){return m_S_deformationElastic_Deviatoric;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get J^{-1/d}F differentiated multiplied by input matrix Z. Ie. B:Z where B is J^{-1/d}F differentiated
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Matrix3f getDeformEDevDiff_Z(const Eigen::Matrix3f &_Z);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get J^{-1/d}F differentiated multiplied by input matrix Z. Ie. Z:B where B is J^{-1/d}F differentiated
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Matrix3f getZ_DeformEDevDiff(const Eigen::Matrix3f &_Z);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Add velocity from grid
  //----------------------------------------------------------------------------------------------------------------------
  inline void addParticleVelocity(Eigen::Vector3f _velocityContribution){m_velocity+=_velocityContribution;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Add to velocity gradient from grid
  //----------------------------------------------------------------------------------------------------------------------
  inline void addParticleVelocityGradient(Eigen::Matrix3f _velocityGradContribution){m_velocityGradient+=_velocityGradContribution;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Add temperature from grid
  //----------------------------------------------------------------------------------------------------------------------
  inline void addParticleTemperature(float _temperatureContribution){m_temperature+=_temperatureContribution;}


  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Function updates the deformation gradients by verifying elastic/plastic contributions. It then updates the
  /// variables for the calculation of deviatoric forces, ie. J^{-1/d}F and so on.
  //----------------------------------------------------------------------------------------------------------------------
  void presetParticlesForTimeStep(float _velocityContribAlpha, float _tempContribBeta);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Update particle. Calls to update velocity, position, temperature and deformation gradient
  /// @param [in] _dt: Time step
  //----------------------------------------------------------------------------------------------------------------------
  void update(float _dt, float _xMin, float _xMax, float _yMin, float _yMax, float _zMin, float _zMax);

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Particle position
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Vector3f m_position;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Particle velocity
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Vector3f m_velocity;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Particle previous velocity (not really needed)
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Vector3f m_previousVelocity;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Particle velocity gradient
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Matrix3f m_velocityGradient;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Particle mass
  //----------------------------------------------------------------------------------------------------------------------
  float m_mass;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Initial density of particle. Used to calculate volume
  //----------------------------------------------------------------------------------------------------------------------
  float m_initialDensity;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Initial volume of particle.
  //----------------------------------------------------------------------------------------------------------------------
  float m_initialVolume;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Elastic deformation gradient, F_E
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Matrix3f m_deformationElastic;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Plastic deformation gradient, F_P;
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Matrix3f m_deformationPlastic;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Actual lame constant mu, taking into account hardening
  //----------------------------------------------------------------------------------------------------------------------
  float m_lameMu;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Actual lame constant lambda, taking into account hardening
  //----------------------------------------------------------------------------------------------------------------------
  float m_lameLambda;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Determinant of deformation gradient F
  //----------------------------------------------------------------------------------------------------------------------
  float m_detDeformGrad;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Determinant of elastic deformation gradient FE
  //----------------------------------------------------------------------------------------------------------------------
  float m_detDeformGradElastic;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Determinant of plastic deformation gradient FP
  //----------------------------------------------------------------------------------------------------------------------
  float m_detDeformGradPlastic;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief d for dimension in deviatoric force calculations
  //----------------------------------------------------------------------------------------------------------------------
  int m_dimension;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief JE^(-1/d)*FE
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Matrix3f m_deformationElastic_Deviatoric;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief R of Polar decomposition of defElastic_Deviatoric
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Matrix3f m_R_deformationElastic_Deviatoric;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief S of Polar decomposition of defElastic_Deviatoric
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Matrix3f m_S_deformationElastic_Deviatoric;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Particle temperature in Kelvin
  //----------------------------------------------------------------------------------------------------------------------
  float m_temperature;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Particle previous temperature
  //----------------------------------------------------------------------------------------------------------------------
  float m_previousTemperature;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Transition heat of particle. Empty (0.0) if solid, full (equal to latent heat) if fluid. Transitioning if
  /// inbetween.
  //----------------------------------------------------------------------------------------------------------------------
  float m_transitionHeat;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Says if particle is solid or liquid.
  //----------------------------------------------------------------------------------------------------------------------
  Phase m_phase;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Emitter that particle belongs to
  //----------------------------------------------------------------------------------------------------------------------
  const Emitter* m_emitter;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Calculate plasticity contribution
  //----------------------------------------------------------------------------------------------------------------------
  void applyPlasticity();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Updates deformation gradient and verifies elastic/plastic contribution
  //----------------------------------------------------------------------------------------------------------------------
  void updateDeformationGradient(float _dt);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Apply phase transition
  //----------------------------------------------------------------------------------------------------------------------
  void applyPhaseTransition();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Resolve collisions. Sets sticking velocity to all particles colliding with surrounding objects
  //----------------------------------------------------------------------------------------------------------------------
  void collisionResolve(float _dt, float _xMin, float _xMax, float _yMin, float _yMax, float _zMin, float _zMax);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Updates position
  //----------------------------------------------------------------------------------------------------------------------
  void updatePosition(float _dt);

};


#endif // PARTICLE

