class Layout_CustomGrid20x19 : BaseLayout
{
	override void Spawn(vector origin)
	{
        const int W = 20;
        const int H = 19;

        // 1) Build grid (E by default)
        array<ref array<int>> grid;
        FP_GridInit(grid, W, H);

        // Rooms + halls (matches your 20x19 pattern)
        FP_GridFillRect(grid,  2,  0, 5, 5, FP_ROOM);
        FP_GridFillRect(grid,  8,  0, 5, 5, FP_ROOM);
        FP_GridFillRect(grid, 16,  0, 4, 5, FP_ROOM);

        FP_GridFillRect(grid,  0,  5,15, 2, FP_HALL);
        FP_GridFillRect(grid, 15,  5, 5, 2, FP_ROOM);

        FP_GridFillRect(grid,  0,  7, 2, 2, FP_HALL);
        FP_GridFillRect(grid,  2,  7, 5, 2, FP_ROOM);
        FP_GridFillRect(grid,  8,  7, 5, 2, FP_ROOM);
        FP_GridFillRect(grid, 13,  7, 2, 2, FP_HALL);
        FP_GridFillRect(grid, 15,  7, 5, 2, FP_ROOM);

        FP_GridFillRect(grid,  0,  9, 2, 1, FP_HALL);
        FP_GridFillRect(grid,  2,  9, 5, 1, FP_ROOM);
        FP_GridFillRect(grid,  8,  9, 5, 1, FP_ROOM);
        FP_GridFillRect(grid, 13,  9, 2, 1, FP_HALL);

        FP_GridFillRect(grid,  0, 10, 2, 2, FP_HALL);
        FP_GridFillRect(grid,  2, 10, 5, 2, FP_ROOM);
        FP_GridFillRect(grid,  8, 10, 5, 2, FP_ROOM);
        FP_GridFillRect(grid, 13, 10, 2, 2, FP_HALL);
        FP_GridFillRect(grid, 15, 10, 5, 2, FP_ROOM);

        FP_GridFillRect(grid,  0, 12,15, 2, FP_HALL);
        FP_GridFillRect(grid, 15, 12, 5, 2, FP_ROOM);

        FP_GridFillRect(grid,  2, 14, 5, 5, FP_ROOM);
        FP_GridFillRect(grid,  8, 14, 5, 5, FP_ROOM);
        FP_GridFillRect(grid, 16, 14, 4, 5, FP_ROOM);

		// 2) Spawn floors for R + H, record floor objects
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid(floorObjs, W, H);

		for (int y = 0; y < H; y++)
		{
			for (int x = 0; x < W; x++)
			{
				int cell = grid[y][x];
				if ( cell != FP_ROOM && cell != FP_HALL ) continue;
				Object f = FP_SpawnFloorAt(origin, x, y);
				floorObjs[y][x] = f;
			}
		}
		
		g_FP_FloorObjs = floorObjs;

        // 3) Doors: one coded doorframe per contiguous room block
        array<ref array<bool>> visited = new array<ref array<bool>>;
        for (int vy = 0; vy < H; vy++)
        {
            auto vrow = new array<bool>;
            vrow.Resize(W);
            visited.Insert(vrow);
        }

        // Collect door edges so wall gen can skip base layer there
        array<ref Param3<int,int,int>> doorEdges = new array<ref Param3<int,int,int>>;

        for (int yy = 0; yy < H; yy++)
        {
            for (int xx = 0; xx < W; xx++)
            {
                if (grid[yy][xx] == FP_ROOM && !visited[yy][xx])
                {
                    FP_PlaceOneDoorForRoomRegion(grid, W, H, origin, xx, yy, visited, doorEdges);
                }
            }
        }

        // Entrance warp door on perimeter (prefer halls), count it as a door edge too
		int egx, egy, edir;
		
		if (FP_FindPerimeterEntrance(grid, W, H, egx, egy, edir))
		{
			Object warpDoor = FP_SpawnWarpDoorOnEdge(origin, egx, egy, edir);
			FP_AddDoorEdge(doorEdges, egx, egy, edir);

			if (warpDoor)
			{
				// read memory point in local coords
				vector mpLocal = "0 0 0";
				if (warpDoor.MemoryPointExists("teleport_point"))
					mpLocal = warpDoor.GetMemoryPointPos("teleport_point");

				// world-space teleport point with current orientation
				vector tpWorld = warpDoor.ModelToWorld(mpLocal);

				// ensure it lands on a walkable cell; if not, flip yaw 180° and recompute
				if (!FP_IsWalkableAtWorld(origin, grid, W, H, tpWorld))
				{
					vector ori = warpDoor.GetOrientation(); // yaw, pitch, roll
					ori[0] = ori[0] + 180.0;                // flip yaw
					warpDoor.SetOrientation(ori);
					// after rotating, recompute world teleport point
					tpWorld = warpDoor.ModelToWorld(mpLocal);
				}

				// store globals for the action
				g_FP_WarpDoorTP_Pos = tpWorld;

				vector doorPos = warpDoor.GetPosition();
				vector dir = tpWorld - doorPos;
				dir[1] = 0.0;
				
				if (dir.Length() > 0.001)
					dir.Normalize();
				else
					dir = warpDoor.GetDirection();

				g_FP_WarpDoorTP_Dir = dir;
			}
		}

        // 4) Walls: base layer + top layer (+3m), skipping base at doors and capping above doorframes
        FP_GenerateWalls(origin, grid, W, H, doorEdges);
		
		// 5) Ceilings: one per walkable cell (rooms + halls), 6m above the floor plane
		for (int cy = 0; cy < H; cy++)
		{
			for (int cx = 0; cx < W; cx++)
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}
		
		// 6) One container per room (centered on a floor tile's 'entity_spawn_point')
		FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
	}
}