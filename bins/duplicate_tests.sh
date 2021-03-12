#!/bin/bash
for i in {1..100}
do
  bazel test --cache_test_results=no ...
done