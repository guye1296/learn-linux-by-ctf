#!/bin/bash
if [ "$PROMPT_CHALLENGE" = "1" ]; then
	mdv $HOME/.description
fi

export PROMPT_CHALLENGE="1"
