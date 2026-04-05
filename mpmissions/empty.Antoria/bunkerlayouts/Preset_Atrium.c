// Spawns an Pre-Built Atrium at x and y position and vertical offset
// Built to catch dir=0,1,2,3 without issue because only 0 and 1 actually matter
// 2 and 3 will end up being the same setup anyways
static void SpawnDIBunkerAtrium( vector origin , int xpos = 0, int ypos = 0, int dir = 0, float voffset = FIRST_FLOOR_LOWER_HEIGHT_OFFSET ) {
	int atriumW; // Set when direction is determined
	int atriumH; // Set when direction is determined
	int nxpos;
	int nypos;
	int randomOutcome;
	if ( dir == 0 || dir == 2 ) {
		atriumW = 8;
		atriumH = 10;
		for ( int i = 0; i < atriumW; i++ ) {
			for ( int j = 0; j < atriumH; j++ ) {
				nxpos = xpos + i;
				nypos = ypos + j;
				if ( i == 0 ) {
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 1.0, voffset + 3, 1.7 );
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 1.0, voffset + 0, 1.7 );
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 1.0, voffset - 3, 1.7 );
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 1.0, voffset - 6, 1.7 );
				}
				if ( i == ( atriumW - 1 ) ) {
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 3.0, voffset + 3, 1.7 );
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 3.0, voffset + 0, 1.7 );
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 3.0, voffset - 3, 1.7 );
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 3.0, voffset - 6, 1.7 );
				}
				if ( i < 2 || i > 5 ) {
					FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_ceiling", 0.0, voffset + 6 );
					FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_floor", 0.0, voffset - 0 );
					FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_floor", 0.0, voffset - 6 );
				} else {
					FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_ceiling", 0.0, voffset + 6 );
					FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_floor", 0.0, voffset - 6 );
				}
				if ( i == 1 ) {
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_glasswall", 1.0, voffset + 0, FP_GLASSRAIL_INSET );
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_glasswall", 1.0, voffset + 3, FP_GLASSRAIL_INSET );
				}
				if ( i == 6 ) {
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_glasswall", 3.0, voffset + 0, FP_GLASSRAIL_INSET );
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_glasswall", 3.0, voffset + 3, FP_GLASSRAIL_INSET );
				}
				if ( j == 0 ) {
					if ( i != 1 && i != 6 ) {
						FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 0.0, voffset + 0, 0.3 );
						FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 0.0, voffset - 6, 0.3 );
					}
					if ( i == 3 || i == atriumW - 4 ) {
						FP_SpawnLabDeskAgainstWall( origin, nxpos, nypos, 0.0, FP_DESK_INSET, voffset - 6 );
						FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_officechair", 0.0, voffset - 6, 1.5 );
					}
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 0.0, voffset + 3, 0.3 );
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 0.0, voffset - 3, 0.3 );
				}
				if ( j == 1 ) {
					if ( i == 0 ) {
						randomOutcome = Math.RandomInt(0,2);
						if ( randomOutcome == 1 ) FP_SpawnGreeble2AgainstWall( origin, nxpos, nypos, 3.0, FP_GREEBLE_2_INSET, voffset );
						else FP_SpawnGreeble1AgainstWall( origin, nxpos, nypos, 3.0, FP_GREEBLE_1_INSET, voffset );
					} 
					if ( i == atriumW - 1 ) {
						randomOutcome = Math.RandomInt(0,2);
						if ( randomOutcome == 1 ) FP_SpawnGreeble2AgainstWall( origin, nxpos, nypos, 1.0, FP_GREEBLE_2_INSET, voffset );
						else FP_SpawnGreeble1AgainstWall( origin, nxpos, nypos, 1.0, FP_GREEBLE_1_INSET, voffset );
					}
				}
				if ( j == 2 ) {
					if ( i == 0 ) {
						FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_static_cafebench", -90.0, voffset - 6 );
					}
					if ( i == 3 || i == 4 ) {
						FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_static_cafetable", 90.0, voffset - 6 );
					}
					if ( i == 7 ) {
						FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_static_cafebench", 90.0, voffset - 6 );
					}
				}
				if ( j == 4 ) {
					if ( i == 0 ) {
						randomOutcome = Math.RandomInt(0,2);
						if ( randomOutcome == 1 ) FP_SpawnGreeble2AgainstWall( origin, nxpos, nypos, 3.0, FP_GREEBLE_2_INSET, voffset );
						else FP_SpawnGreeble1AgainstWall( origin, nxpos, nypos, 3.0, FP_GREEBLE_1_INSET, voffset );
					} 
					if ( i == atriumW - 1 ) {
						randomOutcome = Math.RandomInt(0,2);
						if ( randomOutcome == 1 ) FP_SpawnGreeble2AgainstWall( origin, nxpos, nypos, 1.0, FP_GREEBLE_2_INSET, voffset );
						else FP_SpawnGreeble1AgainstWall( origin, nxpos, nypos, 1.0, FP_GREEBLE_1_INSET, voffset );
					}
				}

				if ( j == 5 ) {
					if ( i == 0 ) {
						randomOutcome = Math.RandomInt(0,2);
						if ( randomOutcome == 1 ) FP_SpawnGreeble2AgainstWall( origin, nxpos, nypos, 3.0, FP_GREEBLE_2_INSET, voffset );
						else FP_SpawnGreeble1AgainstWall( origin, nxpos, nypos, 3.0, FP_GREEBLE_1_INSET, voffset );
					} 
					if ( i == atriumW - 1 ) {
						randomOutcome = Math.RandomInt(0,2);
						if ( randomOutcome == 1 ) FP_SpawnGreeble2AgainstWall( origin, nxpos, nypos, 1.0, FP_GREEBLE_2_INSET, voffset );
						else FP_SpawnGreeble1AgainstWall( origin, nxpos, nypos, 1.0, FP_GREEBLE_1_INSET, voffset );
					}
				}
				if ( j == 7 ) {
					if ( i == 0 ) {
						FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_static_cafebench", -90.0, voffset - 6 );
					}
					if ( i == 3 || i == 4 ) {
						FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_static_cafetable", 90.0, voffset - 6 );
					}
					if ( i == 7 ) {
						FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_static_cafebench", 90.0, voffset - 6 );
					}
				}
				if ( j == 8 ) {
					if ( i == 0 ) {
						randomOutcome = Math.RandomInt(0,2);
						if ( randomOutcome == 1 ) FP_SpawnGreeble2AgainstWall( origin, nxpos, nypos, 3.0, FP_GREEBLE_2_INSET, voffset );
						else FP_SpawnGreeble1AgainstWall( origin, nxpos, nypos, 3.0, FP_GREEBLE_1_INSET, voffset );
					} 
					if ( i == atriumW - 1 ) {
						randomOutcome = Math.RandomInt(0,2);
						if ( randomOutcome == 1 ) FP_SpawnGreeble2AgainstWall( origin, nxpos, nypos, 1.0, FP_GREEBLE_2_INSET, voffset );
						else FP_SpawnGreeble1AgainstWall( origin, nxpos, nypos, 1.0, FP_GREEBLE_1_INSET, voffset );
					}
				}
				if ( j == ( atriumH - 1 ) ) {
					if ( i != 1 && i != 6 ) {
						FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 2.0, voffset + 0, 0.3 );
						FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 2.0, voffset - 6, 0.3 );
					}
					if ( i == 3 || i == atriumW - 4 ) {
						FP_SpawnLabDeskAgainstWall( origin, nxpos, nypos, 2.0, FP_DESK_INSET, voffset - 6 );
						FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_officechair", 2.0, voffset - 6, 1.5 );
					}
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 2.0, voffset - 3, 0.3 );
					FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 2.0, voffset + 3, 0.3 );
				}
			}
		}
	}
}



	// else if ( dir == 1 || dir == 3 ) { atriumW = 10; atriumH = 6; }
			
			// else if ( dir == 1 || dir == 3 ) {
				// if ( j < 2 || j > 5 ) {
					// FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_floor", 0.0, voffset - 0 );
					// FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_floor", 0.0, voffset - 6 );
					// if ( i == 0 ) {
						// FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 1.0, voffset + 0, 0.3 );
						// FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 1.0, voffset + 3, 0.3 );
					// } else if ( i == 10 ) {
						// FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 3.0, voffset + 0, 0.3 );
						// FP_SpawnObjectOnEdge( origin, nxpos, nypos, "fmdpk_modscifiwalls_halfwall", 3.0, voffset + 3, 0.3 );
					// }
				// } else {
					// FP_SpawnObjectAtGrid( origin, nxpos, nypos, "fmdpk_modscifiwalls_floor", 0.0, voffset - 6 );
				// }
			// }