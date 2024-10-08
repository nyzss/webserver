/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:41:04 by okoca             #+#    #+#             */
/*   Updated: 2024/10/04 21:30:57 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "json.hpp"
#include <vector>
class Cluster
{
private:
	std::vector<Server*> _servers;
	std::vector<Config*> _configs;
public:
	Cluster(const JsonValue &json);
	~Cluster ();
public:
	Cluster &start();

	const std::vector<Config*> &get_configs() const;
	const std::vector<Config*>::const_iterator get_config_by_host(const std::string &host) const;
private:
	void clear();
};
