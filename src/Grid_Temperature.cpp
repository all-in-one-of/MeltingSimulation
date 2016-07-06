#include "Grid.h"

//----------------------------------------------------------------------------------------------------------------------

void Grid::calcTemperature()
{
  /* Outline
  ----------------------------------------------------------------------------------------------------------------
  Set up A and B and T

  Set B

  Set A

  Solve

  Set boundary conditions

  Store result
  ----------------------------------------------------------------------------------------------------------------
  */
}

//----------------------------------------------------------------------------------------------------------------------

void Grid::setUpB_temperature()
{
  /* Outline
  ----------------------------------------------------------------------------------------------------------------
  Loop over grid cells
    Set B[i]=Tc^n/dt

  ----------------------------------------------------------------------------------------------------------------
  */
}

//----------------------------------------------------------------------------------------------------------------------

void Grid::setUpA_temperature()
{
  /* Outline
  ----------------------------------------------------------------------------------------------------------------
  Loop over grid cells

    Loop over grid cells

      Calculate constant

      If c==c'
        sum over faces times two for each
        Add 1/dt
      Else
        If i, j, k only one away from ijk of c==c'
          If empty
            Set to zero and subtract 1*constant from c==c'
          Else
            sum over faces times 1 for each

        Else set to zero

  ----------------------------------------------------------------------------------------------------------------
  */
}

//----------------------------------------------------------------------------------------------------------------------

void Grid::setBoundaryTemperature()
{
  /* Outline
  ----------------------------------------------------------------------------------------------------------------
  Loop over all grid cells
    If colliding
      Set temp to colliding object temp

  ----------------------------------------------------------------------------------------------------------------
  */
}

//----------------------------------------------------------------------------------------------------------------------
