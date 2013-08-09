#ifndef _fasd3P_h
#define _fasd3P_h

#include <phigs/phigs.h>

/*******************************************************************************
 * fasd3_head
 *
 * DESCR:       Get facet head information
 * RETURNS:     N/A
 */

void fasd3_head(
   Pfasd3 *fasd3,
   void *pdata
   );

/*******************************************************************************
 * fasd3_next_edata
 *
 * DESCR:       Get facet edge data
 * NOTE:        Remember to set up Pfasd3->edata to point to a valid
 *              Pedge_data_list structure
 * RETURNS:     N/A
 */

void fasd3_next_edata(
   Pfasd3 *fasd3
   );

/*******************************************************************************
 * fasd3_next_vdata3
 *
 * DESCR:       Get facet vertex data
 * NOTE:        Remember to set up Pfasd3->vdata to point to a valid
 *              Pfacet_vdata_list3 structure
 * RETURNS:     N/A
 */

void fasd3_next_vdata3(
   Pfasd3 *fasd3
   );

#endif /* _fasd3P_h */

