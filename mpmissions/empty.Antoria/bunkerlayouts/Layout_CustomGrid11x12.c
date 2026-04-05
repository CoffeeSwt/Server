class Layout_CustomGrid11x12 : BaseLayout
{
	override void Spawn(vector origin)
	{
		const int W = 11;
		const int H = 12;

		// 1) Build grid (E by default)
		array<ref array<int>> grid;
		FP_GridInit(grid, W, H);

		// 2) Hallways (center band, full width, 2 tiles tall)
		// Rows 5..6 across all columns
		FP_GridFillRect(grid, 0, 5, 11, 2, FP_HALL);

		// 3) Rooms
		// Top band (rows 0..4), two blocks with a 1-tile empty gap at column 5
		FP_GridFillRect(grid, 1, 0, 4, 5, FP_ROOM);  // left top block (cols 1..4)
		FP_GridFillRect(grid, 6, 0, 4, 5, FP_ROOM);  // right top block (cols 6..9)

		// Bottom band (rows 7..11), mirrored
		FP_GridFillRect(grid, 1, 7, 4, 5, FP_ROOM);  // left bottom block
		FP_GridFillRect(grid, 6, 7, 4, 5, FP_ROOM);  // right bottom block

		// 4) Spawn floors for R + H, record floor objects
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
		
		g_FP_FloorObjs = floorObjs;  // <--- add this

		// 5) Doors: one coded doorframe per contiguous room block
		array<ref array<bool>> visited = new array<ref array<bool>>;
		for (int vy = 0; vy < H; vy++)
		{
			auto vrow = new array<bool>;
			vrow.Resize(W);
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

				if (dir.Length() > 0.001)
					dir.Normalize();
				else
					dir = warpDoor.GetDirection();

				g_FP_WarpDoorTP_Dir = dir;
			}
		}

		// 7) Walls: base layer + top layer (+3m), skipping base at doors
		FP_GenerateWalls(origin, grid, W, H, doorEdges);

		// 8) Ceilings & 9) One container per contiguous room region
		for (int cy = 0; cy < H; cy++)
		{
			for (int cx = 0; cx < W; cx++)
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}
		FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
	}
}