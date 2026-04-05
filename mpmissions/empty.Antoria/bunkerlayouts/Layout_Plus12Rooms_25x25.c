class Layout_Plus12Rooms_25x25 : BaseLayout
{
	override void Spawn(vector origin)
	{
		const int W = 25; // ≥ 21x21 as requested
		const int H = 25;

		// 1) Build grid (empty by default)
		array<ref array<int>> grid;
		FP_GridInit(grid, W, H); // helpers provided in your library

		// 2) Main PLUS-SHAPED HALLWAY (2 tiles thick)
		// Vertical spine through center columns 12..13
		FP_GridFillRect(grid, 12, 0, 2, 25, FP_HALL);
		// Horizontal spine through center rows 12..13
		FP_GridFillRect(grid, 0, 12, 25, 2, FP_HALL);

		// ========== ROOMS (each 5x5), three per quadrant ==========
		// NW quadrant (x < 12, y < 12)
		FP_GridFillRect(grid,  7,  1, 5, 5, FP_ROOM); // touches vertical hall at x=11
		FP_GridFillRect(grid,  1,  7, 5, 5, FP_ROOM); // touches horizontal hall at y=11
		FP_GridFillRect(grid,  7,  7, 5, 5, FP_ROOM); // corner-adjacent to both

		// NE quadrant (x > 13, y < 12)
		FP_GridFillRect(grid, 14,  1, 5, 5, FP_ROOM); // touches vertical hall at x=14
		FP_GridFillRect(grid, 14,  7, 5, 5, FP_ROOM); // touches vertical hall at x=14
		FP_GridFillRect(grid, 19,  7, 5, 5, FP_ROOM); // touches horizontal hall at y=11

		// SW quadrant (x < 12, y > 13)
		FP_GridFillRect(grid,  7, 14, 5, 5, FP_ROOM); // touches vertical hall at x=11
		FP_GridFillRect(grid,  1, 14, 5, 5, FP_ROOM); // touches horizontal hall at y=14
		FP_GridFillRect(grid,  7, 20, 5, 5, FP_ROOM); // touches vertical hall at x=11

		// SE quadrant (x > 13, y > 13)
		FP_GridFillRect(grid, 14, 14, 5, 5, FP_ROOM); // corner-adjacent to both
		FP_GridFillRect(grid, 19, 14, 5, 5, FP_ROOM); // touches horizontal hall at y=14
		FP_GridFillRect(grid, 14, 20, 5, 5, FP_ROOM); // touches vertical hall at x=14

		// 3) Spawn floors for rooms + halls and record floor objects
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid(floorObjs, W, H);
		for (int y = 0; y < H; y++)
		{
			for (int x = 0; x < W; x++)
			{
				int cell = grid[y][x];
				if (cell != FP_ROOM && cell != FP_HALL) continue;
				Object f = FP_SpawnFloorAt(origin, x, y);
				floorObjs[y][x] = f;
			}
		}
		g_FP_FloorObjs = floorObjs;

		// 4) Doors: one coded doorframe per contiguous room block
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

		// 5) Entrance warp door on perimeter (prefers halls); add to doorEdges
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

		// 6) Walls: base + top layer (+3m), skipping base at door edges
		FP_GenerateWalls(origin, grid, W, H, doorEdges);

		// 7) Ceilings for every non-empty cell (6m above floor plane)
		for (int cy = 0; cy < H; cy++)
			for (int cx = 0; cx < W; cx++)
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);

		// 8) One container per contiguous room region (uses floor memory points)
		FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
	}
}
