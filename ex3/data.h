#ifndef __DATA_H__
# define __DATA_H__

typedef enum	e_operator
{
  ADD,
  SUB,
  MUL,
  DIV
}		t_operator;

typedef struct	s_data
{
  int		nbr1;
  int		nbr2;
  t_operator	operator;
  int		status;
}		t_data;

#endif
