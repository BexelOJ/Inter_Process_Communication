#!/bin/bash

TRACE="/sys/kernel/debug/tracing"

echo 0 | sudo tee "$TRACE/tracing_on" > /dev/null
echo function | sudo tee "$TRACE/current_tracer" > /dev/null

echo "Generating kernel activity..."

echo 1 | sudo tee "$TRACE/tracing_on" > /dev/null

sleep 3

echo 0 | sudo tee "$TRACE/tracing_on" > /dev/null

echo
echo "========== FTRACE OUTPUT =========="

sudo tail -100 "$TRACE/trace"

echo
echo "===================================="



