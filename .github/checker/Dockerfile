# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Dockerfile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aldubar <aldubar@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/18 11:02:19 by aldubar           #+#    #+#              #
#    Updated: 2021/07/18 11:37:56 by aldubar          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FROM		ubuntu:latest

RUN		apt update && apt install -y clang

WORKDIR		/.github/checker

ENTRYPOINT	[ "/bin/bash", "grademe.sh" ]
