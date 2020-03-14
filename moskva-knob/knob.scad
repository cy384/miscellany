// moskva-knob
// knob for moskva 5
// (C) 2020
// cy384
// licensed under the GPL v2, see LICENSE file

$fn = 100;

module key()
{
    difference()
    {
        // correct: 12.45
        circle(r=12.45, center=true);
        
        intersection()
        {
            // measured: 3.0
            square([100, 3.1], center=true);
            circle(r=2, center=true);
        }
        
        // measured: 1.45
        translate([3.775, -3.225, 0]) circle(d=1.65, center=true);
    }
}

// thumb/wingnut style
difference()
{
    union()
    {
        difference()
        {
            union()
            {
                intersection()
                {
                    translate([0,0,0.6+0.5]) difference()
                    {
                        translate([0,0,0]) sphere(r=12.45, center=true);
                        translate([12.45,0,12.45]) rotate([90,0,0]) scale([0.9,1.2,1]) cylinder(r=12.45,h=100, center=true);
                        translate([-12.45,0,12.45]) rotate([90,0,0]) scale([0.9,1.2,1]) cylinder(r=12.45,h=100, center=true);
                    }
                    
                    translate([0,0,7.5]) cube([100,100,15], center = true);
                    
                    union()
                    {
                    translate([0,7,5]) scale([1,1.65,1.35]) rotate([0,90,0]) cylinder(r=5,h=1000,center=true);
                    translate([0,-7,5]) scale([1,1.65,1.35]) rotate([0,90,0]) cylinder(r=5,h=1000,center=true);
                    }

                }

                translate([0,0,0.6+0.5]) cylinder(r=12.45, center=true, h=1);
            }
            
            
            
            
            
            
            cylinder(d=6.5, h=100, center=true);
        }

        translate([0,0,0]) linear_extrude(height = 1.2, center = true, convexity = 5) key();
        
        // comment this line to remove the 'apron' on the bottom
        // without the 'apron' you can print nicely without supports
         translate([0,0,-1.5]) difference() { cylinder(r=12.45, h=3.0,center=true); cylinder(r=9.45, h=3.0,center=true); }
    }
    translate([3.775, -3.225, 0]) cylinder(d=1.65, h=4, center=true);
}

// large round knob style
/*
difference()
{
intersection()
{
    union(){
        translate([0,0,-(5+0.6)]) linear_extrude(height = 1.2, center = true, convexity = 5) key();
        difference()
        {
        translate([0,0,+(5+0.6)]) linear_extrude(height = 1.2, center = true, convexity = 2) circle(r=12.45, center=true);
                cylinder(d=6.5, h=100, center=true);

        }

scale([1,1,0.5])
difference()
{
    union()
    {
        cylinder(r=11.5, h=20, center=true);
        
        for (i = [15:30:375])
        {
            rotate([0,0,i]) translate([11.5,0,0]) scale([1,1.5,1]) cylinder(r=1,h=20,center=true);
        }
        
    }
    
    for (i = [0:30:360])
    {
        rotate([0,0,i]) translate([11.5,0,0]) scale([1,1.771,1]) cylinder(r=1,h=20,center=true);
    }
    
    cylinder(d=6.5, h=100, center=true);
}
}
scale( [1,1,1.2])sphere(r=12.5, center=true);
}
translate([3.775, -3.225, -3]) cylinder(d=1.65, h=5, center=true);
}
*/
