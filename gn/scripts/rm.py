#!/usr/bin/env python
#
# This file originates from @google/skia and was provided under the terms bellow.
# The original license file used by Google may be found in the NOTICE file.
#
# ORIGINAL DISCLAIMER:
# Copyright 2016 Google Inc.
#
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import os
import shutil
import sys

dst, = sys.argv[1:]

if os.path.exists(dst):
  if os.path.isdir(dst):
    shutil.rmtree(dst)
  else:
    os.remove(dst)
