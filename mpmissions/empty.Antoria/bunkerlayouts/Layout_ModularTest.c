class Layout_ModularTest : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 20;
		const int H = 20;

		// 1) BUILD GRID
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

		// 2) DEFINE HALLWAYS
		FP_GridFillRect( grid,	15,	0,	3,	10,	FP_HALL );

		// 3) DEFINE ROOMS
		FP_GridFillRect( grid,	15,	11,	3,	3,	FP_ROOM );

		// 4) SPAWN FLOORS FOR R + H, RECORD FLOOR OBJECTS
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid( floorObjs, W, H );

		for (int y = 0; y < H; y++)
		{
			for (int x = 0; x < W; x++)
			{
				int cell = grid[y][x];
				if (cell != FP_HALL && cell != FP_ROOM) continue;
				Object f = FP_SpawnFloorAt(origin, x, y);
				floorObjs[y][x] = f;
			}
		}
		
		g_FP_FloorObjs = floorObjs;

		// 5) DOORS: ONE CODED DOORFRAME PER CONTIGUOUS ROOM BLOCK
		array<ref array<bool>> visited = new array<ref array<bool>>;
		for (int vy = 0; vy < H; vy++)
		{
			auto vrow = new array<bool>; vrow.Resize(W);
			visited.Insert(vrow);
		}
		
		array<ref Param3<int,int,int>> doorEdges = new array<ref Param3<int,int,int>>;
		
		for (int yy = 0; yy < H; yy++)
		{
			for (int xx = 0; xx < W; xx++)
			{
				if (grid[yy][xx] == FP_ROOM && !visited[yy][xx])
					FP_PlaceOneDoorForRoomRegion(grid, W, H, origin, xx, yy, visited, doorEdges);
			}
		}

		// 6) ENTRANCE WARP DOOR ON PERIMETER (PREFERS HALLS)
		int egx, egy, edir;
		
		if (FP_FindPerimeterEntrance(grid, W, H, egx, egy, edir))
		{
			Object warpDoor = FP_SpawnWarpDoorOnEdge(origin, egx, egy, edir);
			FP_AddDoorEdge(doorEdges, egx, egy, edir);

			if (warpDoor)
			{
				vector mpLocal = "0 0 0";
				
				if (warpDoor.MemoryPointExists("teleport_point"))
					mpLocal = warpDoor.GetMemoryPointPos("teleport_point");

				vector tpWorld = warpDoor.ModelToWorld(mpLocal);
				
				if (!FP_IsWalkableAtWorld(origin, grid, W, H, tpWorld))
				{
					vector ori = warpDoor.GetOrientation();
					ori[0] = ori[0] + 180.0;
					warpDoor.SetOrientation(ori);
					tpWorld = warpDoor.ModelToWorld(mpLocal);
				}

				g_FP_WarpDoorTP_Pos = tpWorld;
				vector doorPos = warpDoor.GetPosition();
				vector dir = tpWorld - doorPos; dir[1] = 0.0;
				
				if (dir.Length() > 0.001)
					dir.Normalize();
				else
					dir = warpDoor.GetDirection();
				
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

		// 7) WALLS: BASE LAYER + TOP LAYER (+3M), SKIPPING BASE AT DOORS
		FP_GenerateWalls(origin, grid, W, H, doorEdges);

		// 8) Ceilings: 6m above floor plane for every non-empty cell
		for (int cy = 0; cy < H; cy++)
		{
			for (int cx = 0; cx < W; cx++)
			{
				if (grid[cy][cx] == FP_HALL || grid[cy][cx] == FP_ROOM)
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}
		
		// 9) ONE CONTAINER PER ROOM (CENTERED ON A FLOOR TILE'S 'ENTITY_SPAWN_POINT')
			// FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
			
			SpawnDIBunkerAtrium( origin, 0, 0 );
	}
}