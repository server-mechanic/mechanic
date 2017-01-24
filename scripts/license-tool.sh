#!/bin/bash 
#
# Copyright (c) 2016,2017 the Server Mechanic Team.
# Server Mechanic (http://server-mechanic.org)
#
# This file is part of Server Mechanic.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#

action=$1
source_file=$2
license_file=$3

function hasLicense() {
  local source_file=$1
  local license_file=$2

  local license_line_count=$(wc -l $license_file | cut -f1 -d ' ')
  head -$license_line_count $source_file | diff $license_file - 2>/dev/null 1>/dev/null
  local result=$?
  return $result
}

function removeLicense() {
  local source_file=$1
  local license_file=$2
  local tmp_file=$(mktemp)

  local license_line_count=$(wc -l $license_file | cut -f1 -d ' ')
  tail -n +$(($license_line_count+1)) $source_file > $tmp_file
  mv $tmp_file $source_file
}

function addLicense() {
  local source_file=$1
  local license_file=$2
  local tmp_file=$(mktemp)

  cat $license_file > $tmp_file
  cat $source_file >> $tmp_file
  mv $tmp_file $source_file
}

case $action in
  remove)
    hasLicense $source_file $license_file
    matched=$?
    if [ "$matched" = "0" ]; then
      removeLicense $source_file $license_file
    fi
  ;;
  add)
    hasLicense $source_file $license_file
    matched=$?
    if [ "$matched" != "0" ]; then
      addLicense $source_file $license_file
    fi
  ;;
  *)
    echo "$0 add|remove <source> <license"
  ;;
esac

exit 0
