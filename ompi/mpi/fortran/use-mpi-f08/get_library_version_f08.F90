! -*- f90 -*-
!
! Copyright (c) 2010-2014 Cisco Systems, Inc.  All rights reserved.
! Copyright (c) 2010-2012 Los Alamos National Security, LLC.
!               All Rights reserved.
! Copyright (c) 2018-2020 Research Organization for Information Science
!                         and Technology (RIST).  All rights reserved.
! $COPYRIGHT$

#include "mpi-f08-rename.h"

subroutine MPI_Get_library_version_f08(version,resultlen,ierror)
   use :: mpi_f08_types, only : MPI_MAX_LIBRARY_VERSION_STRING
   use :: ompi_mpifh_bindings, only : ompi_get_library_version_f
   use, intrinsic :: ISO_C_BINDING, only : C_INT
   implicit none
   character(len=MPI_MAX_LIBRARY_VERSION_STRING), intent(out) :: version
   integer, intent(out) :: resultlen
   integer, optional, intent(out) :: ierror
   integer :: c_ierror

   call ompi_get_library_version_f(version,resultlen,c_ierror,len(version,KIND=C_INT))
   if (present(ierror)) ierror = c_ierror

end subroutine MPI_Get_library_version_f08
