#!/bin/bash

# Terminate already running bar instances
pkill -f "polybar top"

# Launch
polybar top &
