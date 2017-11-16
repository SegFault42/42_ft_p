#include "common.h"

char	*extract_name_from_path(char *path)
{
	char	*file;
	
	if ((file = ft_strchr(path, '/')) == NULL)
		file = path;
	else
	{
		file = ft_strrchr(path, '/');
		++file;
	}
	return (file);
}

