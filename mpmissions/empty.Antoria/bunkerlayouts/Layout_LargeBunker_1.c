class Layout_LargeBunker_1 : BaseLayout
{
	override void Spawn(vector origin)
	{
		// Big enough grid for a long zig-zag corridor and 10 rooms
		const int W = 40;
		const int H = 28;

		// 1) Build grid (E by default)
		array<ref array<int>> grid;
		FP_GridInit(grid, W, H);

		// 2) ZIG-ZAG HALLWAY (2 tiles thick)
		// V1: down
		FP_GridFillRect(grid,  8,  0, 2, 12, FP_HALL);  // cols 8..9, rows 0..11
		// H1: right
		FP_GridFillRect(grid,  8, 12, 11, 2, FP_HALL);  // cols 8..18, rows 12..13
		// V2: down
		FP_GridFillRect(grid, 17, 14, 2, 10, FP_HALL);  // cols 17..18, rows 14..23
		// H2: left (connects back toward the left side)
		FP_GridFillRect(grid,  6, 22, 12, 2, FP_HALL);  // cols 6..17, rows 22..23
		// V3: down (finish near bottom)
		FP_GridFillRect(grid,  6, 24, 2,  4, FP_HALL);  // cols 6..7, rows 24..27

		// 3) ROOMS: 3×3, attached to the hallway; 5 per side total
		// --- Along V1 (cols 8..9, rows 0..11) ---
		// Left side (x smaller)
		FP_GridFillRect(grid,  5,  0, 3, 3, FP_ROOM);  // y 0..2
		FP_GridFillRect(grid,  5,  6, 3, 3, FP_ROOM);  // y 6..8
		// Right side (x larger)
		FP_GridFillRect(grid, 10,  2, 3, 3, FP_ROOM);  // y 2..4
		FP_GridFillRect(grid, 10,  8, 3, 3, FP_ROOM);  // y 8..10

		// --- Along V2 (cols 17..18, rows 14..23) ---
		// Left side
		FP_GridFillRect(grid, 14, 14, 3, 3, FP_ROOM);  // y 14..16
		FP_GridFillRect(grid, 14, 19, 3, 3, FP_ROOM);  // y 19..21
		// Right side
		FP_GridFillRect(grid, 19, 14, 3, 3, FP_ROOM);  // y 14..16
		FP_GridFillRect(grid, 19, 19, 3, 3, FP_ROOM);  // y 19..21

		// --- Along V3 (cols 6..7, rows 24..27) ---
		// Left side
		FP_GridFillRect(grid,  3, 24, 3, 3, FP_ROOM);
		// Right side
		FP_GridFillRect(grid,  8, 24, 3, 3, FP_ROOM);

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

				// make sure the TP lands in a walkable cell; flip if not
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