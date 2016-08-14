#ifndef SIMULATIONCONTROLLER
#define SIMULATIONCONTROLLER

#include <ngl/Camera.h>

#include "Emitter.h"
#include "Grid.h"
#include "ReadGeo.h"
#include "AlembicExport.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------
/// @file SimulationController.h
/// @brief Singleton class which controls the melting simulation and sets the parameters for the simulation
/// @author Ina M. Sorensen
/// @version 1.0
/// @date 25.06.16
///
/// To do:
/// Doesn't register destructor properly for some reason???
/// Write update and render classes
/// Set proper values for simulation parameters
//------------------------------------------------------------------------------------------------------------------------------------------------------


class SimulationController
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Instance creator
  //----------------------------------------------------------------------------------------------------------------------
  static SimulationController* instance();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Destructor
  //----------------------------------------------------------------------------------------------------------------------
  ~SimulationController();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set render parameters
  //----------------------------------------------------------------------------------------------------------------------
  void setRenderParameters(ngl::Camera* _camera, std::string _shaderName);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get the position of the bounding box
  //----------------------------------------------------------------------------------------------------------------------
  inline Eigen::Vector3f getBoundingBoxPosition(){return m_boundingBoxPosition;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get bounding box size
  //----------------------------------------------------------------------------------------------------------------------
  inline float getBoundingBoxSize(){return m_boundingBoxSize;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get the position of the grid. Different to bounding box because of single layer of cells surrounding the
  /// bounding box.
  //----------------------------------------------------------------------------------------------------------------------
  inline Eigen::Vector3f getGridPosition(){return m_grid->getGridCornerPosition();}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get grid cell size
  //----------------------------------------------------------------------------------------------------------------------
  inline float getGridCellSize(){return m_grid->getGridCellSize();}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get number of grid cells
  //----------------------------------------------------------------------------------------------------------------------
  inline int getNoGridCells(){return m_noCells;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get cell state from grid for visualisation
  //----------------------------------------------------------------------------------------------------------------------
  inline State getGridCellState(int _cellIndex){return m_grid->getCellState(_cellIndex);}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get cell temperature from grid for visualisation
  //----------------------------------------------------------------------------------------------------------------------
  inline float getGridCellTemperature(int _cellIndex){return m_grid->getCellTemperature(_cellIndex);}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get ambient temperature
  //----------------------------------------------------------------------------------------------------------------------
  inline float getAmbientTemperature(){return m_ambientTemperature;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get heat source temperature
  //----------------------------------------------------------------------------------------------------------------------
  inline float getHeatSourceTemperature(){return m_heatSourceTemperature;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Updates the simulation using a set time step.
  //----------------------------------------------------------------------------------------------------------------------
  void update();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Renders particles to visualise the simulation
  //----------------------------------------------------------------------------------------------------------------------
  void render(ngl::Mat4 _modelMatrixCamera);

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor. Private for a singleton
  //----------------------------------------------------------------------------------------------------------------------
  SimulationController();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Instance pointer
  //----------------------------------------------------------------------------------------------------------------------
  static SimulationController* m_instance;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to emitter which contains the particles for a single object
  //----------------------------------------------------------------------------------------------------------------------
  Emitter* m_emitter;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to grid used to do calculations on
  //----------------------------------------------------------------------------------------------------------------------
  Grid* m_grid;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to camera
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Camera* m_camera;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Shader name used to set which shader to use when rendering particles
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_shaderName;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Used to set size of the rendered particles
  //----------------------------------------------------------------------------------------------------------------------
  float m_particleRadius;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Size of simulation time step
  //----------------------------------------------------------------------------------------------------------------------
  float m_simTimeStep;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Measures time since last frame was exported. This is used because simulation will be stepped between
  ///        each frame exported
  //----------------------------------------------------------------------------------------------------------------------
  float m_elapsedTimeAfterFrame;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Counts the total number of frames created and exported
  //----------------------------------------------------------------------------------------------------------------------
  int m_noFrames;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Total number of frames for the simulation
  //----------------------------------------------------------------------------------------------------------------------
  int m_totalNoFrames;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Sets position of origin of grid, set as bottom, left, back corner of grid.
  //----------------------------------------------------------------------------------------------------------------------
  Eigen::Vector3f m_boundingBoxPosition;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Size of one side of grid. Grid is always cubic, ie. equal length for all sides.
  //----------------------------------------------------------------------------------------------------------------------
  float m_boundingBoxSize;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Number of grid cells along one side. Again same number of cells along all sides.
  //----------------------------------------------------------------------------------------------------------------------
  int m_noCells;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Number of particles in the simulation. Currently set to belong to one emitter
  //----------------------------------------------------------------------------------------------------------------------
  int m_noParticles;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Mass of a single particle
  //----------------------------------------------------------------------------------------------------------------------
  float m_particleMass;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Lame constant mu. Depends on the material being simulated
  //----------------------------------------------------------------------------------------------------------------------
  float m_lameMuConstant;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Lame constant lambda. Depends on the material being simulated
  //----------------------------------------------------------------------------------------------------------------------
  float m_lameLambdaConstant;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Hardness coefficient
  //----------------------------------------------------------------------------------------------------------------------
  float m_hardnessCoefficient;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Compression limit sets the compression value above which compression goes from elastic to plastic+elastic.
  /// Depends on the material being simulated
  //----------------------------------------------------------------------------------------------------------------------
  float m_compressionLimit;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Stretch limit sets the stretch value above which stretch goes from elastic to plastic+elastic.
  /// Depends on the material being simulated
  //----------------------------------------------------------------------------------------------------------------------
  float m_stretchLimit;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Heat capacity of solid. Depends on the material being simulated
  //----------------------------------------------------------------------------------------------------------------------
  float m_heatCapacitySolid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Heat capacity of fluid. Depends on the material being simulated
  //----------------------------------------------------------------------------------------------------------------------
  float m_heatCapacityFluid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Heat conductivity of solid. Depends on the material being simulated
  //----------------------------------------------------------------------------------------------------------------------
  float m_heatConductivitySolid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Heat conductivity of fluid. Depends on the material being simulated
  //----------------------------------------------------------------------------------------------------------------------
  float m_heatConductivityFluid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Latent heat is the heat it takes for solid-fluid and fluid-solid conversion to happen.
  /// Depends on the material being simulated
  //----------------------------------------------------------------------------------------------------------------------
  float m_latentHeat;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Freezing or melting temperature of material being simulated
  //----------------------------------------------------------------------------------------------------------------------
  float m_freezingTemperature;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Ambient temperature; temperature of the surrounding air. In celsius
  //----------------------------------------------------------------------------------------------------------------------
  float m_ambientTemperature;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Heat source temperature. In celsius
  //----------------------------------------------------------------------------------------------------------------------
  float m_heatSourceTemperature;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Transfer constant giving relative FLIP and PIC contribution to velocity
  //----------------------------------------------------------------------------------------------------------------------
  float m_velocityContributionAlpha;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Transfer constant giving relative FLIP and PIC contribution to temperature
  //----------------------------------------------------------------------------------------------------------------------
  float m_temperatureContributionBeta;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Name of file to read simulation parameters and particle values from
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_readFileName;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set to true if want to export particle data to alembic file
  //----------------------------------------------------------------------------------------------------------------------
  bool m_isExporting;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Name of alembic file to export particle data to
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_exportFileName;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Alembic exporter pointer
  //----------------------------------------------------------------------------------------------------------------------
//  std::unique_ptr <AlembicExport> m_alembicExporter;
  AlembicExport* m_alembicExporter;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Read in simulation parameters from geo file
  //----------------------------------------------------------------------------------------------------------------------
  void readSimulationParameters();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set up particles from emitter by using default values or reading from file.
  //----------------------------------------------------------------------------------------------------------------------
  void setupParticles();



};

#endif // SIMULATIONCONTROLLER

