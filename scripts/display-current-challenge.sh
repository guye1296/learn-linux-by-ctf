#!/bin/bash
if [ "$PROMPT_CHALLENGE" = "1" ]; then
	/usr/bin/glow -p $HOME/.description
fi

export PROMPT_CHALLENGE="1"
