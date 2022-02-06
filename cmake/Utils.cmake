#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2021 RacoonStudios
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy of this
#// software and associated documentation files (the "Software"), to deal in the Software
#// without restriction, including without limitation the rights to use, copy, modify, merge,
#// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
#// to whom the Software is furnished to do so, subject to the following conditions:
#//
#// The above copyright notice and this permission notice shall be included in all copies or
#// substantial portions of the Software.
#//
#// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
#// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
#// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#// DEALINGS IN THE SOFTWARE.
#////////////////////////////////////////////////////////////////////////////////////////////////////


##################################################
## MACRO: pl_message
##
## Output a build message
##################################################
macro(pl_message msg)
  message(STATUS "* " ${msg})
endmacro()

################################################################################
# MACRO: pl_debug_message
################################################################################
macro(pl_debug_message msg)
  if (PL_DEBUG_BUILD_SYSTEM)
    message(STATUS ${msg})
  endif ()
endmacro()

################################################################################
# MACRO pl_debug_variable
################################################################################
macro(pl_debug_variable var)
  pl_debug_message("${var} : ${${var}}")
endmacro()

##################################################
## MACRO: pl_newline
##
## Output a build message newline
##################################################
macro(pl_newline)
  message(STATUS "*")
endmacro()

##################################################
## MACRO: pl_split
##
## Output a build message split
##################################################
macro(pl_split)
  message(STATUS "********************************************************************************")
endmacro()