// tumbller.scad
// Single-file non-parametric model for Elegoo Tumbller (visual/collision meshes).

export_part = "all";
chassis_length=80; //mm
chassis_width=120;

module wheel() {
  difference() {
    cylinder(h=25, r=32, center=true);
    cylinder(h=30, r=2, center=true);
    translate([0,0,6]) cylinder(h=30, r=22, center=true);
  }
}

module motor() {
  cylinder(h=45, r=18, center=true);
}

module lsupport() {
  rounded_box([40,40,2], 1);
  translate([-20,0,21]) rotate([0,90,0])
  rounded_box([40,40,2], 1);
}

module shaft() {
  cylinder(h=30, r=2, center=true);
}

module plate(width, length) {
  rounded_box([width,length,2], 1);
}

module battery_plate(width, length) {
  plate(width, length);
  translate([width / 2 - 3, length / 2 - 3, -45 / 2 - 1]) cylinder(h=45, r=2, center=true);
  translate([-(width / 2 - 3), -(length / 2 - 3), -45 / 2 - 1]) cylinder(h=45, r=2, center=true);
  translate([width / 2 - 3, -(length / 2 - 3), -45 / 2 - 1]) cylinder(h=45, r=2, center=true);
  translate([-(width / 2 - 3), length / 2 - 3, -45 / 2 - 1]) cylinder(h=45, r=2, center=true);
}

module top_plate(width, length) {
  support_height = 22;
  plate(width, length);
  translate([width / 2 - 3, length / 2 - 3, -support_height / 2 - 1]) cylinder(h=support_height, r=2, center=true);
  translate([-(width / 2 - 3), -(length / 2 - 3), -support_height / 2 - 1]) cylinder(h=support_height, r=2, center=true);
  translate([width / 2 - 3, -(length / 2 - 3), -support_height / 2 - 1]) cylinder(h=support_height, r=2, center=true);
  translate([-(width / 2 - 3), length / 2 - 3, -support_height / 2 - 1]) cylinder(h=support_height, r=2, center=true);
}

module rounded_box(size=[100,50,20], r=5) {
  minkowski() {
    cube(size, center=true);
    sphere(r=r);
  }
}

module pcb() {
  h=5;
  translate([0,0,0]) cube([95,80,2], center=true);
  // short sensor bump
  translate([25,0,4]) cube([20,12,8], center=true);
  translate([95 / 2 - 10, 80 / 2 - 5, -6]) cylinder(h=h, r=2, center=true);
  translate([- (95 / 2 - 10), 80 / 2 - 5, -6]) cylinder(h=5, r=2, center=true);
  translate([95 / 2 - 10, -(80 / 2 - 5), -6]) cylinder(h=h, r=2, center=true);
  translate([-(95 / 2 - 10), -(80 / 2 - 5), -6]) cylinder(h=h, r=2, center=true);
}

module battery() {
  rounded_box([80,55,22], 1);
}

module kickstand() {
  h=5;
  rounded_box([55,68,2], 1);
  translate([0,68 / 2 -1,5]) rotate([90,0,0])
  rounded_box([55,10,2], 1);
  translate([55 / 2 - 3, -(68 / 2 - 3), h-1]) cylinder(h=h, r=2, center=true);
  translate([-(55 / 2 - 3), -(68 / 2 - 3), h-1]) cylinder(h=h, r=2, center=true);
}

module tumbller_assembly() {
  // Chassis
  translate([0,0,0]) plate(chassis_width, chassis_length);
  
  translate([0, 68 - 2, -8]) kickstand();

  translate([-60, 0, -22]) rotate([0, 90, 0]) lsupport();
  translate([60, 0, -22]) rotate([0, 90, 180]) lsupport();

  // Motors
  translate([-36, 0, -25]) rotate([0, 90, 0]) motor();
  translate([36, 0, -25]) rotate([0, -90, 0]) motor();

  // Shafts
  translate([-76, 0, -32]) rotate([0, 90, 0]) shaft();
  translate([76, 0, -32]) rotate([0, -90, 0]) shaft();
  
  // Battery plate
  translate([0,0,46]) battery_plate(chassis_width, chassis_length);

  // Top
  translate([0,0,46 + 24]) top_plate(chassis_width, chassis_length);
  
  // Left wheel
  translate([79,0,-32]) rotate([0,-90,0]) wheel();
  // Right wheel
  translate([-79,0,-32]) rotate([0,90,0]) wheel();

  // Battery on top
  translate([0,0,56]) battery();

  // PCB
  translate([0,0,10]) pcb();
}

// ---------- Export selection helper ----------
if (export_part == "all") {
  tumbller_assembly();
} else if (export_part == "chassis") {
  plate();
} else if (export_part == "battery_plate") {
  plate();
} else if (export_part == "top") {
  plate();
} else if (export_part == "wheel") {
  wheel();
} else if (export_part == "pcb") {
  pcb();
} else if (export_part == "battery") {
  battery();
} else if (export_part == "kickstand") {
  kickstand();
} else if (export_part == "motor") {
  motor();
} else if (export_part == "lsupport") {
  lsupport();
} else if (export_part == "shaft") {
  shaft();
}