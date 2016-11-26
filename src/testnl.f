	program testnl
C
C	Get namelist to work.
C
	integer x(6), y(6)
	namelist /bigloop/ x, y 

	read (5,nml=bigloop)
	write(6,600) (x(j), y(j), j=1,6)
 600	format(12(1x,i4))
	stop
	end
