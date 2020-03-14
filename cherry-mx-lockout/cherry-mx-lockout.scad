// cherry-mx-lockout.scad
// lockout for 1U cherry mx switches
// (C) 2020
// cy384
// licensed under the GPL v2, see LICENSE file

module lockout_shape()
{
	difference()
	{
		union()
		{
			difference()
			{
				square([18.5, 18.5], center=true);
				square([14.5, 14.5], center=true);
			}
			
			for(i=[-6.25, 6.25], j=[-6.25, 6.25])
			{
				translate([i, j]) { square([2, 4], center=true); }
				translate([i, 4.25]) { square([2, 2], center=true); }
			}
		}
		
		rotate([0, 0, 45]) { square([18.5, 18.5], center=false); }
	}
}

linear_extrude(height=3.5, center=true, convexity=6)
{
	lockout_shape();
}
