/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:33:49 by ggalizon          #+#    #+#             */
/*   Updated: 2025/08/04 17:44:51 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	main(int ac, char **av)
{
	t_table	table;
	int		status;

	memset(&table, 0, sizeof(t_table));
	status = 0;
	if (!check_input(ac, av))
		status = 1;
	else if (!data_init(av, &table))
		status = 1;
	else if (!start(&table))
		status = 1;
	if (!cleanup(&table))
		status = 1;
	return (status);
}
