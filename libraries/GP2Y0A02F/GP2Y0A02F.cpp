/**
 * \file GP2Y0A02F.cpp
 * \author Rüdiger Härtel
 *
 */


/* header of standard C - libraries */
#ifdef GP2YA02F_DUMP
#include <stdio.h>
#endif
#include <string.h>
#include <math.h>

/* header of common types */

/* shared common header */

/* header of project specific types */

/* project headers */

/* local header */
#include "GP2Y0A02F.h"

/* constant definitions
---------------------------------------------------------------------------*/
#ifndef countof
# define countof( x ) (sizeof(x) / sizeof(x[0]) )
#endif

/* local defined data types
---------------------------------------------------------------------------*/
struct _LUT {
  uint16_t ADvalue;
  int      Slope;
  int      Offset;
};

/* list of external used functions, if not in headers
---------------------------------------------------------------------------*/

/* list of global defined functions
---------------------------------------------------------------------------*/

/* list of local defined functions
---------------------------------------------------------------------------*/
static uint16_t Map( uint16_t );

/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_FILE_ID
static char _fileid[] = "$Id$";
#endif

/**
 * Calculation is based on a table that has offset and slope
 * for some A/D values. For A/D values not in this list
 * the distance is interpolated.
 * 
 * The slope is multiplied by 16 to gain some more precsion.
 * The factor 64 was chosen to simplify the calculation hence
 * a division by 64 can be implemented by a right shift of 6.
 */
static struct _LUT _5V_LUT[] =
{
  { 409, (int16_t)(-11.38 * 64), 751 },
  { 307, (int16_t)( -9.31 * 64), 689 },
  { 204, (int16_t)( -4.65 * 64), 498 },
  { 122, (int16_t)( -2.21 * 64), 344 },
  {   0, (int16_t)( -0.68 * 64), 191 },
};


/*** GP2Y0A02F *************************************************************/
/**
 *  Constructor
 * 
 *  \param Pin    Pin of the analogue digital converter
 */
GP2Y0A02F::GP2Y0A02F( int Pin )
{
  _pin = Pin;
}

/*** begin *****************************************************************/
/**
 *  Initialize internals
 * 
 *  At the moment this function is empty and only here because the style
 *  guide recommens it.
 * 
 */
void GP2Y0A02F::begin( )
{
}

/*** GetDistanceCm *********************************************************/
/**
 *  Calculate the distance
 * 
 *  The value is read from the analogue pin and converted to a distance.
 * 
 *  \return distance in cm
 */
uint16_t GP2Y0A02F::getDistanceCm()
{
  return getDistanceCm( getDistanceRaw() );
}

/*** GetDistanceCm *********************************************************/
/**
 *  Calculate the distance
 * 
 *  \param ADValue    Value from analog to digital converter
 * 
 *  \return distance in cm
 */
uint16_t GP2Y0A02F::getDistanceCm(uint16_t ADValue)
{
  return Map( ADValue );
}

/*** GetDistanceRaw ********************************************************/
/**
 *  Reads from the analoue pin.
 * 
 *  On Linux this functions return 0.
 * 
 *  \return value from the analogue pin
 */
uint16_t GP2Y0A02F::getDistanceRaw()
{
#ifdef linux
  return 0;
#else
  return (uint16_t) analogRead( _pin );
#endif
}

/*** Map ****************************************************************/
/**
 *  Calculate the distance
 * 
 *  \param ADValue    Value from analog to digital converter
 * 
 *  \return distance in cm
 */
uint16_t Map( uint16_t ADValue)
{
  uint16_t u;
  int16_t Dist = 0;
  struct _LUT *p = _5V_LUT;
  
  if( ADValue < 100 )
  {
    return 130;
  }
  
  if( ADValue > 512 )
  {
    return 21;
  }
  
  for( u = 0; u < countof(_5V_LUT); u++ )
  {
    if( ADValue >= p->ADvalue )
    {
      Dist = (int16_t) ADValue - p->Offset;
#ifdef GP2YA02F_DUMP
      printf("%u:  Ofs/Slope/Dist: %d/%d/%d\n", u, p->Offset, p->Slope, Dist );
#endif
      // shifting by 6 means divide by 64
      // adding 32 corrects rounding
      Dist = (uint16_t) ( ((Dist<<6) + 32) / p->Slope );
      break;
    }
    p++;
  }
  
  return Dist;
}


/*_____________________________________________________________________EOF_*/
