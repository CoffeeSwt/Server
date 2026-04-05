class Layout_LargeBunker_2 : BaseLayout
{
	override void Spawn(vector origin)
	{
		const int W = 30;
		const int H = 30;

		// 1) Build grid (E by default)
		array<ref array<int>> grid;
		FP_GridInit(grid, W, H);

		// 2) HALLWAY — U shape (2 tiles thick), touches top edge for warp entrance
		// Left vertical leg
		FP_GridFillRect(grid,  6,  0, 2, 25, FP_HALL); // x=6..7,  y=0..24
		// Right vertical leg
		FP_GridFillRect(grid, 22,  0, 2, 25, FP_HALL); // x=22..23,y=0..24
		// Bottom base connecting the legs
		FP_GridFillRect(grid,  6, 22, 18, 2, FP_HALL); // x=6..23, y=22..23

		// 3) ROOMS — 10 total, each 4×4, separated by E so each is its own block.
		// Left side (outside of left leg)
		FP_GridFillRect(grid,  2,  2, 4, 4, FP_ROOM);
		FP_GridFillRect(grid,  2,  7, 4, 4, FP_ROOM);
		FP_GridFillRect(grid,  2, 12, 4, 4, FP_ROOM);
		FP_GridFillRect(grid,  2, 17, 4, 4, FP_ROOM);

		// Right side (outside of right leg)
		FP_GridFillRect(grid, 24,  2, 4, 4, FP_ROOM);
		FP_GridFillRect(grid, 24,  7, 4, 4, FP_ROOM);
		FP_GridFillRect(grid, 24, 12, 4, 4, FP_ROOM);
		FP_GridFillRect(grid, 24, 17, 4, 4, FP_ROOM);

		// Along the bottom, outside the base
		FP_GridFillRect(grid, 10, 24, 4, 4, FP_ROOM);
		FP_GridFillRect(grid, 16, 24, 4, 4, FP_ROOM);

		// 4) Spawn floors for rooms + halls and record floor objects
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

		// 5) Doors: one coded doorframe per contiguous room block
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

		// 6) Entrance warp door on perimeter (prefers halls)
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
				if (dir.Length() > 0.001) dir.Normalize(); else dir = warpDoor.GetDirection();
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

		// 7) Walls (base + top layers, skip base at door edges)
		FP_GenerateWalls(origin, grid, W, H, doorEdges);

		// 8) Ceilings for every non-empty cell
		for (int cy = 0; cy < H; cy++)
		{
			for (int cx = 0; cx < W; cx++)
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}

		// 9) One container per contiguous room region
		FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
	}
}
