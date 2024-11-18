#!/usr/bin/env bash

RUNNABLE="${1}"
[[ ! -x ${RUNNABLE} ]] && {
  echo "The provided argument \"${RUNNABLE}\" is not an executable!"
  exit 1
}

DATFILE="${RUNNABLE/.exe/.dat}"
[[ ! -f ${DATFILE} ]] && {
  echo "There is no \"${DATFILE}\" in the current directory!"
  exit 2
}

./"${RUNNABLE}" "$(< ${DATFILE})"
