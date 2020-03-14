// industar-lens-cap
// cap for the industar 69 half frame lens
// (C) 2020
// cy384
// licensed under the GPL v2, see LICENSE file
$fn = 64;

fudge_factor = 0.0;

module cap()
{
	rotate([0, 180, 0])
	{
		difference()
		{
			translate([0, 0, 21])
			{
                cylinder(h=35, r1=6, r=6, center=true);
			}

			cylinder(h = 39.12, r = (1.588/2) + fudge_factor, center = false);
		}
	}
}

difference()
{
    translate([0,0,-1]) cylinder(r=(45.5/2)+1, h=3+2, center=true);
	cylinder(r=45.45/2, h=3, center=true);
    
	for (rot = [0, 60, 120])
	{
		rotate([0,0,rot]) cube([1,100,3], center = true);
	}
}
