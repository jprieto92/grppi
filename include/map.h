/**
* @version		GrPPI v0.2
* @copyright		Copyright (C) 2017 Universidad Carlos III de Madrid. All rights reserved.
* @license		GNU/GPL, see LICENSE.txt
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You have received a copy of the GNU General Public License in LICENSE.txt
* also available in <http://www.gnu.org/licenses/gpl.html>.
*
* See COPYRIGHT.txt for copyright notices and details.
*/

#ifndef GRPPI_MAP_H
#define GRPPI_MAP_H

#include "common/common.h"

#include "seq/map.h"

#include "native/map.h"

#include "omp/map.h"

#include "tbb/map.h"

#include "poly/map.h"

#if 0 /* START DOCUMENTATION */
/** @addtogroup BDataPattern
 *  @{
 */
/** @defgroup Map
 *
 *  @brief Apply the map pattern for parallelizing the code section
 *
 *  The Map pattern is applied to iterable data structures in which all the
 *  the elements are processed the same way, with the same 'op' function.
 *  This function parallelize the execution of the 'op' function in different
 *  elements of the input data structure.
 *
 *  @{
 */
/** @param exec     Execution_model flag to indicates the type of execution
 *    (sequential or parallel) and the implementation framework.
 *  @param first    Iterator to the first element we want to modify in the
 *    iterable datastructure
 *  @param last     Iterator to the last element we want to modify in the 
 *    iterable datastructure
 *  @param firstOut Iterator to the first element where we want to store the
 *    result of the operations
 *  @param tasf     Task function: function that will contain the section of
 *    code that will be applied to each elements of the input data structure.
 *    This section of code will be parallelize.
 */
template <typename InputIt, typename OutputIt, typename Operation>
 void Map(execution_model exec, InputIt first, InputIt last, OutputIt firstOut, Operation && op );


/**  @param exec     Execution_model flag to indicates the type of execution
 *    (sequential or parallel) and the implementation framework.
 *  @param first    Iterator to the first element we want to modify in the
 *    iterable datastructure
 *  @param last     Iterator to the last element we want to modify in the 
 *    iterable datastructure
 *  @param firstOut Iterator to the first element where we want to store the
 *    result of the operations
 *  @param tasf     Task function: function that will contain the section of
 *    code that will be applied to each elements of the input data structure.
 *    This section of code will be parallelize.
 *  @param inputs   List of additional inputs to the function 'op'
 *  
 */
template <typename InputIt, typename OutputIt, typename ... MoreIn, typename Operation>
 void Map(execution_model exec, InputIt first, InputIt last, OutputIt firstOut, Operation && op, MoreIn ... inputs ) {
/** @} */
/** @} */
#endif /* END DOCUMENTATION */

#endif
