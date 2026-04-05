// Global floor grid used by spawners/utilities
static ref array<ref array<Object>> g_FP_FloorObjs;

// ZOMBIE WAVE HANDLING
const int FP_ZED_WAVE_SIZE = 10;

// CUSTOM DUNGEON HELPER CLASSES =============================
const float FP_TILE_SIZE     				= 2.0;
const float FP_HEIGHT_OFFSET 				= 10.0;

const float FP_WALL_INSET	 				= 0.30;
const float FP_DOORFRAME_INSET	 			= 0.00;

const float FP_LOCKER_INSET	 				= 0.75;
const float FP_MEDICAL_INSET 				= 0.80;
const float FP_DESK_INSET	 				= 0.95;
const float FP_REFRIGERATOR_INSET			= 0.00;
const float FP_CONTAINER_INSET				= 0.95;
const float FP_GEARSTORAGE_INSET			= 0.95;
const float FP_STORAGESHELF_INSET			= 0.85;

const float FP_GLASSRAIL_INSET				= 0.05;
const float FP_GLASSRAIL_INSET_2			= 1.95;
const float FP_GREEBLE_1_INSET				= 0.45;
const float FP_GREEBLE_2_INSET				= 0.60;
const float FP_WETFLOORSIGN_INSET			= 0.50;

const float FIRST_FLOOR_LOWER_HEIGHT_OFFSET = 10;
const float FIRST_FLOOR_UPPER_HEIGHT_OFFSET = 13;
const float FIRST_FLOOR_PIT_HEIGHT_OFFSET_1	= 1;
const float FIRST_FLOOR_PIT_HEIGHT_OFFSET_2	= 4;
const float FIRST_FLOOR_PIT_HEIGHT_OFFSET_3	= 7;

//DoorFrame Cell Types
const int FP_EMPTY = 0;
const int FP_ROOM  = 1;
const int FP_HALL  = 2;

//Exit Vectors
static vector g_FP_WarpDoorTP_Pos = "0 0 0";
static vector g_FP_WarpDoorTP_Dir = "0 0 1";

static Object FP_SpawnFloorAt(vector origin, int gx, int gy)
{
	vector pos = Vector(origin[0] + gx * FP_TILE_SIZE, origin[1] + FP_HEIGHT_OFFSET, origin[2] + gy * FP_TILE_SIZE);
	return SpawnDIBunkerObject("fmdpk_modscifiwalls_floor", pos, "0 0 0", 1.0);
}

// Generic: spawn any object at a grid tile center with optional offsets/orientation.
static Object FP_SpawnObjectAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject(className, pos, ori, scale);
}

// spawn lockers at a grid tile center with optional offsets/orientation.
static Object FP_SpawnLockersAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_lockers", pos, ori, scale);
}

// spawn a medical cabinet at a grid tile center with optional offsets/orientation.
static Object FP_SpawnMedicalAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_medicalcab", pos, ori, scale);
}

// spawn a desk at a grid tile center with optional offsets/orientation.
static Object FP_SpawnLabDeskAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_labdesk", pos, ori, scale);
}

// spawn gear storage at a grid tile center with optional offsets/orientation.
static Object FP_SpawnGearStorageAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_gearstorage", pos, ori, scale);
}

// spawn coded doorframe at a grid tile center with optional offsets/orientation.
static Object FP_SpawnCodedDoorAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_doorframe_coded", pos, ori, scale);
}

// spawn wide coded doorframe at a grid tile center with optional offsets/orientation.
static Object FP_SpawnWideCodedDoorAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_doorframe_coded_wide", pos, ori, scale);
}

// spawn coded laser doorframe at a grid tile center with optional offsets/orientation.
static Object FP_SpawnCodedLaserDoorAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_doorframe_laser", pos, ori, scale);
}

// spawn wide coded laser doorframe at a grid tile center with optional offsets/orientation.
static Object FP_SpawnWideCodedLaserDoorAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_doorframe_laser_wide", pos, ori, scale);
}

// spawn medical bed at a grid tile center with optional offsets/orientation.
static Object FP_SpawnMedicalBedAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_static_medicalbed", pos, ori, scale);
}

// spawn wet floor sign at a grid tile center with optional offsets/orientation.
static Object FP_SpawnWetFloorSignAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_static_wetfloorsign", pos, ori, scale);
}

// spawn storage shelf at a grid tile center with optional offsets/orientation.
static Object FP_SpawnStorageShelfAtGrid( vector origin, int gx, int gy, string className, float yaw = 0.0, float yOffset = FP_HEIGHT_OFFSET, float ox = 0.0, float oz = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE; // grid → world scale
    vector pos = Vector(origin[0] + gx * t + ox, origin[1] + yOffset, origin[2] + gy * t + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_static_storageshelf", pos, ori, scale);
}

// Generic: spawn any object on a specific edge of a grid cell (0=N,1=E,2=S,3=W).
static Object FP_SpawnObjectOnEdge( vector origin, int gx, int gy, string className, int dir, float yOffset = FP_HEIGHT_OFFSET, float inset = 0.0, float scale = 1.0)
{
    float t = FP_TILE_SIZE;
    float wx, wz, yaw;
    float ox = 0.0, oz = 0.0;

    if (dir == 0)      { wx = origin[0] + gx * t;          wz = origin[2] + (gy - 0.5) * t;  yaw = 0;    oz =  inset; }
    else if (dir == 2) { wx = origin[0] + gx * t;          wz = origin[2] + (gy + 0.5) * t;  yaw = 180;  oz = -inset; }
    else if (dir == 1) { wx = origin[0] + (gx + 0.5) * t;  wz = origin[2] + gy * t;          yaw = 90;   ox = -inset; }
    else               { wx = origin[0] + (gx - 0.5) * t;  wz = origin[2] + gy * t;          yaw = -90;  ox =  inset; }

    vector pos = Vector(wx + ox, origin[1] + yOffset, wz + oz);
    vector ori = Vector(yaw, 0, 0);
    return SpawnDIBunkerObject(className, pos, ori, scale);
}


// Store floor objects by grid tile so we can read memory points later
static void FP_InitFloorObjGrid(out array<ref array<Object>> floors, int W, int H)
{
	floors = new array<ref array<Object>>;
	for (int y = 0; y < H; y++)
	{
		auto row = new array<Object>;
		row.Resize(W);
		floors.Insert(row);
	}
}

// helper to start the zone after the layout has set g_FP_WarpDoorTP_Pos
static void FP_StartBunkerZone()
{
    if (!GetGame().IsServer()) return;
    // waveSize = 20 as requested
    g_FP_BunkerZone = new BunkerAIZone(g_FP_WarpDoorTP_Pos, 22.0, 20, 5 * 60 * 1000);
}

// Delete exactly one floor tile at (gx,gy). Safe on bounds & non-walkables.
// Keeps floorObjs and g_FP_FloorObjs in sync so spawners don't target "air".
static void FP_DeleteFloorTile(int gx, int gy, int W, int H, array<ref array<int>> grid, array<ref array<Object>> floorObjs)
{
    if (!GetGame().IsServer())
        return;

    if (gx < 0 || gx >= W || gy < 0 || gy >= H)
        return;

    // (Optional extra safety)
    if (!grid || gy >= grid.Count() || !grid[gy] || gx >= grid[gy].Count())
        return;
	
    if (!floorObjs || gy >= floorObjs.Count() || !floorObjs[gy] || gx >= floorObjs[gy].Count())
        return;

    int cell = grid[gy][gx];
    if (cell != FP_ROOM && cell != FP_HALL)
        return;

    Object f = floorObjs[gy][gx];
    if (!f)
        return;

    GetGame().ObjectDelete(f);
    floorObjs[gy][gx] = null;

    if (g_FP_FloorObjs)
        g_FP_FloorObjs[gy][gx] = null;
}

// Returns true if any *alive* infected are within radius of center (server-side)
static bool FP_HaveInfectedWithin(vector center, float radius)
{
    if (!GetGame().IsServer()) return false;

    array<Object> objs = new array<Object>;
    GetGame().GetObjectsAtPosition3D(center, radius, objs, null);

    for (int i = 0; i < objs.Count(); i++)
    {
        DayZInfected z = DayZInfected.Cast(objs[i]);
        if (z && z.IsAlive())
            return true;
    }
    return false;
}

static void SpawnWaveAtRandomFloors(int count /*=20*/)
{
    if (!GetGame().IsServer()) return;
    if (!g_FP_FloorObjs || g_FP_FloorObjs.Count() == 0) return;

    // Flatten floors
    ref array<Object> allFloors = new array<Object>;
    int Y = g_FP_FloorObjs.Count();
    int X, x, y;
    Object tile;
    for (y = 0; y < Y; y++)
    {
        X = g_FP_FloorObjs[y].Count();
        for (x = 0; x < X; x++)
        {
            tile = g_FP_FloorObjs[y][x];
            if (tile) allFloors.Insert(tile);
        }
    }
    if (allFloors.Count() == 0) return;

    // Infected pool
    ref array<string> pool = new array<string>;
    pool.Insert("ZmbM_NBC_White");
    pool.Insert("ZmbM_NBC_Yellow");
    pool.Insert("ZmbM_NBC_Grey");

    // Local vars reused
    Object chosen;
    vector mpLocal, pos, ori;
    DayZInfected z;
    string ztype;
    int pi, idx, spawned = 0, attempts = 0, maxAttempts = count * 10; // plenty of retries

    // Keep trying random floors until we actually spawn 'count' zeds
    while (spawned < count && attempts < maxAttempts)
    {
        attempts++;
        if (allFloors.Count() == 0) break;

        idx = Math.RandomInt(0, allFloors.Count());
        chosen = allFloors[idx];
        if (!chosen) continue;

        if (chosen.MemoryPointExists("entity_spawn_point"))
        {
            mpLocal = chosen.GetMemoryPointPos("entity_spawn_point");
            pos     = chosen.ModelToWorld(mpLocal);
        }
        else
        {
            pos = chosen.GetPosition();
        }

        // slight lift; do NOT PlaceOnSurface() (snaps to terrain)
        pos[1] = pos[1] + 0.05;
        ori    = chosen.GetOrientation();

        pi    = Math.RandomInt(0, pool.Count());
        ztype = pool.Get(pi);

        // Remove ECE_NOLIFETIME so bodies don't insta-cleanup
        z = DayZInfected.Cast(GetGame().CreateObjectEx(ztype, pos, ECE_CREATEPHYSICS | ECE_KEEPHEIGHT));
        if (z)
        {
            z.SetOrientation(ori);
            spawned++;
        }
        else
        {
            // If this floor keeps failing, drop it from candidates and try another
            allFloors.Remove(idx);
        }
    }
}

// Generic spawn on a floor's memory point (falls back to tile center)
static void FP_SpawnItemOnFloor(Object floorObj, string className)
{
	if (!floorObj) return;

	vector pos;
	if (floorObj.MemoryPointExists("entity_spawn_point"))
		pos = floorObj.ModelToWorld(floorObj.GetMemoryPointPos("entity_spawn_point"));
	else
		pos = floorObj.GetPosition();

	vector ori = floorObj.GetOrientation();
	SpawnDIBunkerObject(className, pos, ori, 1.0);
}

// Try to spawn a container on a floor object at its memory point
static void FP_SpawnContainerOnFloor(Object floorObj)
{
	if (!floorObj) return;

	vector localMP = "0 0 0";
	if (floorObj.MemoryPointExists("entity_spawn_point"))
		localMP = floorObj.GetMemoryPointPos("entity_spawn_point");
	
	vector worldPos = floorObj.ModelToWorld(localMP);
	vector ori      = floorObj.GetOrientation();
	
	// Spawn the container at the memory point; no persistence/lifetime
	SpawnDIBunkerObject("fmdpk_modscifiwalls_container", worldPos, ori, 1.0);
}

static void FP_PlaceContainerForRoomRegion( array<ref array<int>> grid, int W, int H, vector origin, int sx, int sy, inout array<ref array<bool>> visited, array<ref array<Object>> floors, array<ref Param3<int,int,int>> doorEdges )
{
	ref array<ref Param2<int,int>> q = new array<ref Param2<int,int>>;
	ref array<ref Param2<int,int>> region = new array<ref Param2<int,int>>;

	int minx = sx, maxx = sx, miny = sy, maxy = sy;

	q.Insert(new Param2<int,int>(sx, sy));
	visited[sy][sx] = true;

	int dx4[4] = {0, 1, 0, -1};
	int dy4[4] = {-1, 0, 1, 0};

	// Flood-fill
	while (q.Count() > 0)
	{
		Param2<int,int> n = q.Get(q.Count() - 1);
		q.Remove(q.Count() - 1);
		region.Insert(new Param2<int,int>(n.param1, n.param2));

		if (n.param1 < minx) minx = n.param1;
		if (n.param1 > maxx) maxx = n.param1;
		if (n.param2 < miny) miny = n.param2;
		if (n.param2 > maxy) maxy = n.param2;

		for (int d = 0; d < 4; d++)
		{
			int nx = n.param1 + dx4[d];
			int ny = n.param2 + dy4[d];
			if (nx < 0 || nx >= W || ny < 0 || ny >= H) continue;
			if (grid[ny][nx] != FP_ROOM || visited[ny][nx]) continue;
			visited[ny][nx] = true;
			q.Insert(new Param2<int,int>(nx, ny));
		}
	}

	// Center container (avoid door edges)
	float cxF = (minx + maxx) * 0.5;
	float cyF = (miny + maxy) * 0.5;
	int bestX = -1, bestY = -1;
	float bestDist = 1e9;

	for (int i = 0; i < region.Count(); i++)
	{
		int rx = region[i].param1;
		int ry = region[i].param2;
		if (FP_TileTouchesDoorEdge(rx, ry, doorEdges)) continue;

		float dist = Math.AbsFloat(rx - cxF) + Math.AbsFloat(ry - cyF);
		if (dist < bestDist)
		{
			bestDist = dist;
			bestX = rx; bestY = ry;
		}
	}
	if (bestX == -1) { bestX = Math.Round(cxF); bestY = Math.Round(cyF); }

	Object floorCenter = floors[bestY][bestX];
	FP_SpawnContainerOnFloor(floorCenter);

	// Extra props if region is big
	int roomW = (maxx - minx + 1);
	int roomH = (maxy - miny + 1);
	if (roomW >= 5 && roomH >= 5)
	{
		int mx = Math.Round((minx + maxx) * 0.5);
		int my = Math.Round((miny + maxy) * 0.5);

		int tryGX[4] = { mx,  maxx, mx,   minx };
		int tryGY[4] = { miny, my,   maxy, my   };
		int tryDir[4]= { 0,    1,    2,    3    };

		bool picked = false;
		int egx2 = mx, egy2 = miny, edir = 0;

		for (int t = 0; t < 4 && !picked; t++)
		{
			int gx2 = tryGX[t];
			int gy2 = tryGY[t];
			int dir2 = tryDir[t];

			int nx2 = gx2 + (dir2 == 1) - (dir2 == 3);
			int ny2 = gy2 + (dir2 == 2) - (dir2 == 0);
			bool neighborNonRoom = (nx2 < 0 || nx2 >= W || ny2 < 0 || ny2 >= H) || (grid[ny2][nx2] != FP_ROOM);

			if (neighborNonRoom && !FP_IsDoorEdge(gx2, gy2, dir2, doorEdges) && !FP_TileTouchesDoorEdge(gx2, gy2, doorEdges))
			{
				egx2 = gx2; egy2 = gy2; edir = dir2;
				picked = true;
			}
		}
		if (!picked) { egx2 = mx; egy2 = miny; edir = 0; }

		// Locker
		if (!FP_TileTouchesDoorEdge(egx2, egy2, doorEdges))
			FP_SpawnLockerAgainstWall(origin, egx2, egy2, edir);

		// Lab desk
		int deskGX = -1, deskGY = -1; float dBest = 1e9;
		if (edir == 0 || edir == 2)
		{
			int wy;
			if (edir == 0) wy = miny; else wy = maxy;
			for (int wxi = minx + 1; wxi <= maxx - 1; wxi++)
			{
				if ((wxi == egx2 && wy == egy2) || grid[wy][wxi] != FP_ROOM) continue;
				if (FP_IsDoorEdge(wxi, wy, edir, doorEdges)) continue;
				if (FP_TileTouchesDoorEdge(wxi, wy, doorEdges)) continue;
				float dd = Math.AbsFloat(wxi - ((minx + maxx) * 0.5));
				if (dd < dBest) { dBest = dd; deskGX = wxi; deskGY = wy; }
			}
		}
		else
		{
			int wx;
			if (edir == 1) wx = maxx; else wx = minx;
			for (int wyi = miny + 1; wyi <= maxy - 1; wyi++)
			{
				if ((wx == egx2 && wyi == egy2) || grid[wyi][wx] != FP_ROOM) continue;
				if (FP_IsDoorEdge(wx, wyi, edir, doorEdges)) continue;
				if (FP_TileTouchesDoorEdge(wx, wyi, doorEdges)) continue;
				float dd2 = Math.AbsFloat(wyi - ((miny + maxy) * 0.5));
				if (dd2 < dBest) { dBest = dd2; deskGX = wx; deskGY = wyi; }
			}
		}
		if (deskGX != -1) FP_SpawnLabDeskAgainstWall(origin, deskGX, deskGY, edir);

		// Medical cabinet (opposite wall)
		int odir = (edir + 2) % 4;
		int mgx = -1, mgy = -1;
		int omx = Math.Round((minx + maxx) * 0.5);
		int omy = Math.Round((miny + maxy) * 0.5);

		if (odir == 0) { mgx = omx; mgy = miny; }
		else if (odir == 2) { mgx = omx; mgy = maxy; }
		else if (odir == 1) { mgx = maxx; mgy = omy; }
		else { mgx = minx; mgy = omy; }

		bool spawnedMedical = false;
		if (mgx != -1 && !FP_TileTouchesDoorEdge(mgx, mgy, doorEdges) && !FP_IsDoorEdge(mgx, mgy, odir, doorEdges))
		{
			FP_SpawnMedicalAgainstWall(origin, mgx, mgy, odir);
			spawnedMedical = true;
		}

		// Gear storage (same wall as medical)
		if (spawnedMedical)
		{
			int ggx = -1, ggy = -1;
			if (odir == 0 || odir == 2)
			{
				int wy4;
				if (odir == 0) wy4 = miny; else wy4 = maxy;
				for (int delta = 1; delta <= (maxx - minx); delta++)
				{
					int cx1 = mgx - delta;
					int cx2 = mgx + delta;
					if (cx1 > minx && grid[wy4][cx1] == FP_ROOM && !FP_TileTouchesDoorEdge(cx1, wy4, doorEdges) && !FP_IsDoorEdge(cx1, wy4, odir, doorEdges)) { ggx = cx1; ggy = wy4; break; }
					if (cx2 < maxx && grid[wy4][cx2] == FP_ROOM && !FP_TileTouchesDoorEdge(cx2, wy4, doorEdges) && !FP_IsDoorEdge(cx2, wy4, odir, doorEdges)) { ggx = cx2; ggy = wy4; break; }
				}
			}
			else
			{
				int wx4;
				if (odir == 1) wx4 = maxx; else wx4 = minx;
				for (int delta2 = 1; delta2 <= (maxy - miny); delta2++)
				{
					int cy1 = mgy - delta2;
					int cy2 = mgy + delta2;
					if (cy1 > miny && grid[cy1][wx4] == FP_ROOM && !FP_TileTouchesDoorEdge(wx4, cy1, doorEdges) && !FP_IsDoorEdge(wx4, cy1, odir, doorEdges)) { ggx = wx4; ggy = cy1; break; }
					if (cy2 < maxy && grid[cy2][wx4] == FP_ROOM && !FP_TileTouchesDoorEdge(wx4, cy2, doorEdges) && !FP_IsDoorEdge(wx4, cy2, odir, doorEdges)) { ggx = wx4; ggy = cy2; break; }
				}
			}
			if (ggx != -1) FP_SpawnGearStorageAgainstWall(origin, ggx, ggy, odir);
		}

        // Storage shelf opposite the medical cabinet
        if (spawnedMedical)
        {
            int sgx = -1;
            int sgy = -1;

            if (odir == 0 || odir == 2)
            {
                int wy5;
                if (odir == 0)
                    wy5 = miny;
                else
                    wy5 = maxy;

                for (int delta3 = 1; delta3 <= (maxx - minx); delta3++)
                {
                    int cx = mgx + delta3;
                    if (cx <= maxx && grid[wy5][cx] == FP_ROOM && !FP_TileTouchesDoorEdge(cx, wy5, doorEdges) && !FP_IsDoorEdge(cx, wy5, odir, doorEdges))
                    {
                        sgx = cx;
                        sgy = wy5;
                        break;
                    }
                }
            }
            else
            {
                int wx5;
                if (odir == 1)
                    wx5 = maxx;
                else
                    wx5 = minx;

                for (int delta4 = 1; delta4 <= (maxy - miny); delta4++)
                {
                    int cy = mgy + delta4;
                    if (cy <= maxy && grid[cy][wx5] == FP_ROOM && !FP_TileTouchesDoorEdge(wx5, cy, doorEdges) && !FP_IsDoorEdge(wx5, cy, odir, doorEdges))
                    {
                        sgx = wx5;
                        sgy = cy;
                        break;
                    }
                }
            }

            if (sgx != -1)
            {
                FP_SpawnStorageShelfAgainstWall(origin, sgx, sgy, odir);
            }
        }

        // Two shelves along the east wall (dir = 1)
        for (int sy2 = miny + 1; sy2 <= maxy - 1; sy2++)
        {
            int ex = maxx;
            if (grid[sy2][ex] == FP_ROOM && !FP_TileTouchesDoorEdge(ex, sy2, doorEdges) && !FP_IsDoorEdge(ex, sy2, 1, doorEdges))
            {
                // First shelf
                FP_SpawnStorageShelfAgainstWall(origin, ex, sy2, 1);

                // Second shelf directly below, if valid
                if (sy2 + 1 <= maxy &&  grid[sy2 + 1][ex] == FP_ROOM && !FP_TileTouchesDoorEdge(ex, sy2 + 1, doorEdges) && !FP_IsDoorEdge(ex, sy2 + 1, 1, doorEdges))
                {
                    FP_SpawnStorageShelfAgainstWall(origin, ex, sy2 + 1, 1);
                }

                break; // done after placing shelves
            }
        }

	}
}

// True if any of the 4 edges of (gx,gy) is a door edge
static bool FP_TileTouchesDoorEdge(int gx, int gy, array<ref Param3<int,int,int>> doorEdges)
{
	for (int d = 0; d < 4; d++)
		if (FP_IsDoorEdge(gx, gy, d, doorEdges))
			return true;
	return false;
}

// Place exactly one container per contiguous FP_ROOM region (now door-aware)
static void FP_SpawnContainersForAllRooms( array<ref array<int>> grid, int W, int H, vector origin, array<ref array<Object>> floors, array<ref Param3<int,int,int>> doorEdges )
{
	array<ref array<bool>> visited = new array<ref array<bool>>;
	for (int y = 0; y < H; y++)
	{
		auto row = new array<bool>; row.Resize(W);
		visited.Insert(row);
	}

	for (int yy = 0; yy < H; yy++)
	{
		for (int xx = 0; xx < W; xx++)
		{
			if (grid[yy][xx] == FP_ROOM && !visited[yy][xx])
			{
				FP_PlaceContainerForRoomRegion(grid, W, H, origin, xx, yy, visited, floors, doorEdges);
			}
		}
	}
}

static void FP_PlaceRoomNxN(vector origin, int startGX, int startGY, int n /*=5*/)
{
	for (int dx = 0; dx < n; dx++)
		for (int dy = 0; dy < n; dy++)
			FP_SpawnFloorAt(origin, startGX + dx, startGY + dy);
}

static void FP_PlaceHallH(vector origin, int startGX, int startGY, int lengthTiles, int width /*=2*/)
{
	for (int i = 0; i < lengthTiles; i++)
		for (int w = 0; w < width; w++)
			FP_SpawnFloorAt(origin, startGX + i, startGY + w);
}

static void FP_PlaceHallV(vector origin, int startGX, int startGY, int lengthTiles, int width /*=2*/)
{
	for (int i = 0; i < lengthTiles; i++)
		for (int w = 0; w < width; w++)
			FP_SpawnFloorAt(origin, startGX + w, startGY + i);
}

static void FP_FillRect(vector origin, int startGX, int startGY, int width, int height)
{
	for (int dx = 0; dx < width; dx++)
		for (int dy = 0; dy < height; dy++)
			FP_SpawnFloorAt(origin, startGX + dx, startGY + dy);
}

static void FP_GridInit(out array<ref array<int>> grid, int w, int h)
{
    grid = new array<ref array<int>>;
    for (int y = 0; y < h; y++)
    {
        auto row = new array<int>;
        row.Resize(w);
        grid.Insert(row);
    }
}

static void FP_GridFillRect(array<ref array<int>> grid, int x, int y, int w, int h, int val)
{
    for (int dx = 0; dx < w; dx++)
        for (int dy = 0; dy < h; dy++)
            grid[y + dy][x + dx] = val;
}

static void FP_SpawnDoorOnEdge(vector origin, int gx, int gy, int dir /*0=N,1=E,2=S,3=W*/)
{
    float t = FP_TILE_SIZE;
    float y = origin[1] + FP_HEIGHT_OFFSET;

    float wx, wz, yaw;

    if (dir == 0)
    {   wx = origin[0] + gx * t;     wz = origin[2] + (gy - 0.5) * t;  yaw = 0;   }
    else if (dir == 2)
    {   wx = origin[0] + gx * t;     wz = origin[2] + (gy + 0.5) * t;  yaw = 0;   }
    else if (dir == 1)
    {   wx = origin[0] + (gx + 0.5) * t;  wz = origin[2] + gy * t;     yaw = 90;  }
    else
    {   wx = origin[0] + (gx - 0.5) * t;  wz = origin[2] + gy * t;     yaw = 90;  }

    vector pos = Vector(wx, y, wz);
    vector ori = Vector(yaw, 0, 0);
	
	int chosenDoor = Math.RandomInt(0,2);
	
	if ( chosenDoor )
		SpawnDIBunkerObject("fmdpk_modscifiwalls_doorframe_coded", pos, ori, 1.0);
	else
		SpawnDIBunkerObject("fmdpk_modscifiwalls_doorframe_laser", pos, ori, 1.0);
}

static bool FP_PlaceOneDoorForRoomRegion(array<ref array<int>> grid, int w, int h, vector origin, int sx, int sy, inout array<ref array<bool>> visited, inout array<ref Param3<int,int,int>> doorEdges)
{
    ref array<ref Param2<int,int>> q = new array<ref Param2<int,int>>;
    q.Insert(new Param2<int,int>(sx, sy));
    visited[sy][sx] = true;

    int edgeGX = -1;
    int edgeGY = -1;
    int edgeDir = -1;

    int dx[4]; int dy[4];
    dx[0] =  0; dy[0] = -1; // N
    dx[1] =  1; dy[1] =  0; // E
    dx[2] =  0; dy[2] =  1; // S
    dx[3] = -1; dy[3] =  0; // W

    while (q.Count() > 0)
    {
        Param2<int,int> node = q.Get(q.Count() - 1);
        q.Remove(q.Count() - 1);
        int x = node.param1;
        int y = node.param2;

        for (int d = 0; d < 4; d++)
        {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;

            int nval = grid[ny][nx];
            if (nval == FP_HALL && edgeDir == -1)
            {
                edgeGX = x;
                edgeGY = y;
                edgeDir = d;
            }

            if (nval == FP_ROOM && !visited[ny][nx])
            {
                visited[ny][nx] = true;
                q.Insert(new Param2<int,int>(nx, ny));
            }
        }
    }

    if (edgeDir != -1)
    {
        FP_SpawnDoorOnEdge(origin, edgeGX, edgeGY, edgeDir);
        FP_AddDoorEdge(doorEdges, edgeGX, edgeGY, edgeDir);
        return true;
    }
    return false;
}

// Store door edges picked by room regions (gx, gy, dir)
static void FP_AddDoorEdge(inout array<ref Param3<int,int,int>> doorEdges, int gx, int gy, int dir)
{
    doorEdges.Insert(new Param3<int,int,int>(gx, gy, dir));
}

static bool FP_IsSameEdge(int gx, int gy, int dir, int ox, int oy, int odir)
{
    if (gx == ox && gy == oy && dir == odir)
        return true;

    int ogx = ox, ogy = oy, od = odir;
    if (od == 0) { ogy = oy - 1; od = 2; }
    else if (od == 2) { ogy = oy + 1; od = 0; }
    else if (od == 1) { ogx = ox + 1; od = 3; }
    else if (od == 3) { ogx = ox - 1; od = 1; }

    return gx == ogx && gy == ogy && dir == od;
}

static bool FP_IsDoorEdge(int gx, int gy, int dir, array<ref Param3<int,int,int>> doorEdges)
{
    for (int i = 0; i < doorEdges.Count(); i++)
    {
        Param3<int,int,int> e = doorEdges[i];
        if (FP_IsSameEdge(gx, gy, dir, e.param1, e.param2, e.param3))
            return true;
    }
    return false;
}



// Spawn a halfwall along a cell edge, inset into the current cell by `inset` meters.
// dir: 0=N,1=E,2=S,3=W
static void FP_SpawnWallOnEdge(vector origin, int gx, int gy, int dir, float layerY, float inset /*=0.3*/)
{
    float t = FP_TILE_SIZE;

    float wx, wz, yaw;
    float ox = 0.0;
    float oz = 0.0;

    if (dir == 0) // N
    {
        wx = origin[0] + gx * t;
        wz = origin[2] + (gy - 0.5) * t;
        yaw = 0;
        oz = inset;
    }
    else if (dir == 2) // S
    {
        wx = origin[0] + gx * t;
        wz = origin[2] + (gy + 0.5) * t;
        yaw = 180;
        oz = 0.0 - inset;
    }
    else if (dir == 1) // E
    {
        wx = origin[0] + (gx + 0.5) * t;
        wz = origin[2] + gy * t;
        yaw = -90;           // same as 270
        ox = 0.0 - inset;
    }
    else // W
    {
        wx = origin[0] + (gx - 0.5) * t;
        wz = origin[2] + gy * t;
        yaw = 90;
        ox = inset;
    }

    vector pos = Vector(wx + ox, layerY, wz + oz);
    vector ori = Vector(yaw, 0, 0);
    SpawnDIBunkerObject("fmdpk_modscifiwalls_halfwall", pos, ori, 1.0);
}

// Spawn a container flush against the given cell edge (0=N,1=E,2=S,3=W)
// Inset slightly so it sits inside the room, not clipping through the wall.
static void FP_SpawnConatinerAgainstWall(vector origin, int gx, int gy, int dir, float inset = FP_CONTAINER_INSET, float hoffset = FP_HEIGHT_OFFSET )
{
    float t = FP_TILE_SIZE;
    float y = origin[1] + hoffset;

    float wx, wz, yaw;
    float ox = 0.0, oz = 0.0;

    if (dir == 0) // N wall (top edge of the cell)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy - 0.5) * t;  yaw = 0;    oz = inset;       }
    else if (dir == 2) // S wall (bottom edge)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy + 0.5) * t;  yaw = 180;  oz = -inset;      }
    else if (dir == 1) // E wall (right edge)
    {   wx = origin[0] + (gx + 0.5) * t;   wz = origin[2] + gy * t;          yaw = 90;   ox = -inset;      }
    else // 3 = W wall (left edge)
    {   wx = origin[0] + (gx - 0.5) * t;   wz = origin[2] + gy * t;          yaw = -90;  ox = inset;       }

    vector pos = Vector(wx + ox, y, wz + oz);
    vector ori = Vector(yaw, 0, 0);

    SpawnDIBunkerObject("fmdpk_modscifiwalls_container", pos, ori, 1.0);
}

// Spawn a locker flush against the given cell edge (0=N,1=E,2=S,3=W)
// Inset slightly so it sits inside the room, not clipping through the wall.
static void FP_SpawnLockerAgainstWall(vector origin, int gx, int gy, int dir, float inset = FP_LOCKER_INSET, float hoffset = FP_HEIGHT_OFFSET )
{
    float t = FP_TILE_SIZE;
    float y = origin[1] + hoffset;

    float wx, wz, yaw;
    float ox = 0.0, oz = 0.0;

    if (dir == 0) // N wall (top edge of the cell)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy - 0.5) * t;  yaw = 0;    oz = inset;       }
    else if (dir == 2) // S wall (bottom edge)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy + 0.5) * t;  yaw = 180;  oz = -inset;      }
    else if (dir == 1) // E wall (right edge)
    {   wx = origin[0] + (gx + 0.5) * t;   wz = origin[2] + gy * t;          yaw = 90;   ox = -inset;      }
    else // 3 = W wall (left edge)
    {   wx = origin[0] + (gx - 0.5) * t;   wz = origin[2] + gy * t;          yaw = -90;  ox = inset;       }

    vector pos = Vector(wx + ox, y, wz + oz);
    vector ori = Vector(yaw, 0, 0);

    SpawnDIBunkerObject("fmdpk_modscifiwalls_lockers", pos, ori, 1.0);
}

// Spawn a medical cabinet flush against the given cell edge (0=N,1=E,2=S,3=W)
// Inset slightly so it sits inside the room, not clipping through the wall.
static void FP_SpawnMedicalAgainstWall( vector origin, int gx, int gy, int dir, float inset = FP_MEDICAL_INSET, float hoffset = FP_HEIGHT_OFFSET )
{
    float t = FP_TILE_SIZE;
    float y = origin[1] + hoffset;

    float wx, wz, yaw;
    float ox = 0.0, oz = 0.0;

    if (dir == 0) // N wall (top edge of the cell)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy - 0.5) * t;  yaw = 0;    oz = inset;       }
    else if (dir == 2) // S wall (bottom edge)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy + 0.5) * t;  yaw = 180;  oz = -inset;      }
    else if (dir == 1) // E wall (right edge)
    {   wx = origin[0] + (gx + 0.5) * t;   wz = origin[2] + gy * t;          yaw = 90;   ox = -inset;      }
    else // 3 = W wall (left edge)
    {   wx = origin[0] + (gx - 0.5) * t;   wz = origin[2] + gy * t;          yaw = -90;  ox = inset;       }

    vector pos = Vector(wx + ox, y, wz + oz);
    vector ori = Vector(yaw, 0, 0);

    SpawnDIBunkerObject("fmdpk_modscifiwalls_medicalcab", pos, ori, 1.0);
}

// Spawn a desk flush against the given cell edge (0=N,1=E,2=S,3=W)
// Inset slightly so it sits inside the room, not clipping through the wall.
static void FP_SpawnLabDeskAgainstWall( vector origin, int gx, int gy, int dir, float inset = FP_DESK_INSET, float hoffset = FP_HEIGHT_OFFSET )
{
    float t = FP_TILE_SIZE;
    float y = origin[1] + hoffset;

    float wx, wz, yaw;
    float ox = 0.0, oz = 0.0;

    if (dir == 0) // N wall (top edge of the cell)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy - 0.5) * t;  yaw = 0;    oz = inset;       }
    else if (dir == 2) // S wall (bottom edge)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy + 0.5) * t;  yaw = 180;  oz = -inset;      }
    else if (dir == 1) // E wall (right edge)
    {   wx = origin[0] + (gx + 0.5) * t;   wz = origin[2] + gy * t;          yaw = 90;   ox = -inset;      }
    else // 3 = W wall (left edge)
    {   wx = origin[0] + (gx - 0.5) * t;   wz = origin[2] + gy * t;          yaw = -90;  ox = inset;       }

    vector pos = Vector(wx + ox, y, wz + oz);
    vector ori = Vector(yaw, 0, 0);

    SpawnDIBunkerObject("fmdpk_modscifiwalls_labdesk", pos, ori, 1.0);
}

// Spawn a greeble flush against the given cell edge (0=N,1=E,2=S,3=W)
// Inset slightly so it sits inside the room, not clipping through the wall.
static void FP_SpawnGreeble1AgainstWall( vector origin, int gx, int gy, int dir, float inset = FP_GREEBLE_1_INSET, float hoffset = FP_HEIGHT_OFFSET )
{
    float t = FP_TILE_SIZE;
    float y = origin[1] + hoffset;

    float wx, wz, yaw;
    float ox = 0.0, oz = 0.0;

    if (dir == 0) // N wall (top edge of the cell)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy - 0.5) * t;  yaw = 0;    oz = inset;       }
    else if (dir == 2) // S wall (bottom edge)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy + 0.5) * t;  yaw = 180;  oz = -inset;      }
    else if (dir == 1) // E wall (right edge)
    {   wx = origin[0] + (gx + 0.5) * t;   wz = origin[2] + gy * t;          yaw = 90;   ox = -inset;      }
    else // 3 = W wall (left edge)
    {   wx = origin[0] + (gx - 0.5) * t;   wz = origin[2] + gy * t;          yaw = -90;  ox = inset;       }

    vector pos = Vector(wx + ox, y, wz + oz);
    vector ori = Vector(yaw, 0, 0);

    SpawnDIBunkerObject("fmdpk_modscifiwalls_static_greeble_1", pos, ori, 1.0);
}

// Spawn a greeble flush against the given cell edge (0=N,1=E,2=S,3=W)
// Inset slightly so it sits inside the room, not clipping through the wall.
static void FP_SpawnGreeble2AgainstWall( vector origin, int gx, int gy, int dir, float inset = FP_GREEBLE_2_INSET, float hoffset = FP_HEIGHT_OFFSET )
{
    float t = FP_TILE_SIZE;
    float y = origin[1] + hoffset;

    float wx, wz, yaw;
    float ox = 0.0, oz = 0.0;

    if (dir == 0) // N wall (top edge of the cell)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy - 0.5) * t;  yaw = 0;    oz = inset;       }
    else if (dir == 2) // S wall (bottom edge)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy + 0.5) * t;  yaw = 180;  oz = -inset;      }
    else if (dir == 1) // E wall (right edge)
    {   wx = origin[0] + (gx + 0.5) * t;   wz = origin[2] + gy * t;          yaw = 90;   ox = -inset;      }
    else // 3 = W wall (left edge)
    {   wx = origin[0] + (gx - 0.5) * t;   wz = origin[2] + gy * t;          yaw = -90;  ox = inset;       }

    vector pos = Vector(wx + ox, y, wz + oz);
    vector ori = Vector(yaw, 0, 0);

    SpawnDIBunkerObject("fmdpk_modscifiwalls_static_greeble_2", pos, ori, 1.0);
}

// Spawn a wet floor sign flush against the given cell edge (0=N,1=E,2=S,3=W)
// Inset slightly so it sits inside the room, not clipping through the wall.
static void FP_SpawnWetFloorSignAgainstWall( vector origin, int gx, int gy, int dir, float inset = FP_WETFLOORSIGN_INSET, float hoffset = FP_HEIGHT_OFFSET )
{
    float t = FP_TILE_SIZE;
    float y = origin[1] + hoffset;

    float wx, wz, yaw;
    float ox = 0.0, oz = 0.0;

    if (dir == 0) // N wall (top edge of the cell)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy - 0.5) * t;  yaw = 0;    oz = inset;       }
    else if (dir == 2) // S wall (bottom edge)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy + 0.5) * t;  yaw = 180;  oz = -inset;      }
    else if (dir == 1) // E wall (right edge)
    {   wx = origin[0] + (gx + 0.5) * t;   wz = origin[2] + gy * t;          yaw = 90;   ox = -inset;      }
    else // 3 = W wall (left edge)
    {   wx = origin[0] + (gx - 0.5) * t;   wz = origin[2] + gy * t;          yaw = -90;  ox = inset;       }

    vector pos = Vector(wx + ox, y, wz + oz);
    vector ori = Vector(yaw, 0, 0);

    SpawnDIBunkerObject("fmdpk_modscifiwalls_static_wetfloorsign", pos, ori, 1.0);
}

// Spawn a storage shelf flush against the given cell edge (0=N,1=E,2=S,3=W)
// Inset slightly so it sits inside the room, not clipping through the wall.
static void FP_SpawnStorageShelfAgainstWall( vector origin, int gx, int gy, int dir, float inset = FP_STORAGESHELF_INSET, float hoffset = FP_HEIGHT_OFFSET )
{
    float t = FP_TILE_SIZE;
    float y = origin[1] + hoffset;

    float wx, wz, yaw;
    float ox = 0.0, oz = 0.0;

    if (dir == 0) // N wall (top edge of the cell)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy - 0.5) * t;  yaw = 0;    oz = inset;       }
    else if (dir == 2) // S wall (bottom edge)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy + 0.5) * t;  yaw = 180;  oz = -inset;      }
    else if (dir == 1) // E wall (right edge)
    {   wx = origin[0] + (gx + 0.5) * t;   wz = origin[2] + gy * t;          yaw = 90;   ox = -inset;      }
    else // 3 = W wall (left edge)
    {   wx = origin[0] + (gx - 0.5) * t;   wz = origin[2] + gy * t;          yaw = -90;  ox = inset;       }

    vector pos = Vector(wx + ox, y, wz + oz);
    vector ori = Vector(yaw, 0, 0);

    SpawnDIBunkerObject("fmdpk_modscifiwalls_static_storageshelf", pos, ori, 1.0);
}

// Spawn a refrigerator flush against the given cell edge (0=N,1=E,2=S,3=W)
// Inset slightly so it sits inside the room, not clipping through the wall.
// static void FP_SpawnRefrigeratorAgainstWall( vector origin, int gx, int gy, int dir, float inset = FP_REFRIGERATOR_INSET, float hoffset = FP_HEIGHT_OFFSET )
// {
    // float t = FP_TILE_SIZE;
    // float y = origin[1] + hoffset;

    // float wx, wz, yaw;
    // float ox = 0.0, oz = 0.0;

    // if (dir == 0) // N wall (top edge of the cell)
    // {   wx = origin[0] + gx * t;           wz = origin[2] + (gy - 0.5) * t;  yaw = 0;    oz = inset;       }
    // else if (dir == 2) // S wall (bottom edge)
    // {   wx = origin[0] + gx * t;           wz = origin[2] + (gy + 0.5) * t;  yaw = 180;  oz = -inset;      }
    // else if (dir == 1) // E wall (right edge)
    // {   wx = origin[0] + (gx + 0.5) * t;   wz = origin[2] + gy * t;          yaw = 90;   ox = -inset;      }
    // else // 3 = W wall (left edge)
    // {   wx = origin[0] + (gx - 0.5) * t;   wz = origin[2] + gy * t;          yaw = -90;  ox = inset;       }

    // vector pos = Vector(wx + ox, y, wz + oz);
    // vector ori = Vector(yaw, 0, 0);

    // SpawnDIBunkerObject("fmdpk_modscifiwalls_refrigerator", pos, ori, 1.0);
// }

// Spawn a refrigerator flush against the given cell edge (0=N,1=E,2=S,3=W)
// Inset slightly so it sits inside the room, not clipping through the wall.
static void FP_SpawnGearStorageAgainstWall( vector origin, int gx, int gy, int dir, float inset = FP_GEARSTORAGE_INSET, float hoffset = FP_HEIGHT_OFFSET )
{
    float t = FP_TILE_SIZE;
    float y = origin[1] + hoffset;

    float wx, wz, yaw;
    float ox = 0.0, oz = 0.0;

    if (dir == 0) // N wall (top edge of the cell)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy - 0.5) * t;  yaw = 0;    oz = inset;       }
    else if (dir == 2) // S wall (bottom edge)
    {   wx = origin[0] + gx * t;           wz = origin[2] + (gy + 0.5) * t;  yaw = 180;  oz = -inset;      }
    else if (dir == 1) // E wall (right edge)
    {   wx = origin[0] + (gx + 0.5) * t;   wz = origin[2] + gy * t;          yaw = 90;   ox = -inset;      }
    else // 3 = W wall (left edge)
    {   wx = origin[0] + (gx - 0.5) * t;   wz = origin[2] + gy * t;          yaw = -90;  ox = inset;       }

    vector pos = Vector(wx + ox, y, wz + oz);
    vector ori = Vector(yaw, 0, 0);

    SpawnDIBunkerObject("fmdpk_modscifiwalls_gearstorage", pos, ori, 1.0);
}

// Generate walls on all edges; skip base-layer if the edge has a door; always place second layer.
static void FP_GenerateWalls(vector origin, array<ref array<int>> grid, int W, int H, array<ref Param3<int,int,int>> doorEdges)
{
    float yBase = origin[1] + FP_HEIGHT_OFFSET;
    float yTop  = yBase + 3.0; // walls/doorframes are 3m high

    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            int cell = grid[y][x];
			if ( cell != FP_ROOM && cell != FP_HALL )
				continue;

            for (int d = 0; d < 4; d++)
            {
                int nx = x + (d == 1) - (d == 3);
                int ny = y + (d == 2) - (d == 0);

                int nval = FP_EMPTY;
                if (nx >= 0 && nx < W && ny >= 0 && ny < H)
                    nval = grid[ny][nx];

                bool differentArea = (nval != cell);
                bool needsWall     = (nval == FP_EMPTY) || differentArea;
                if (!needsWall) continue;

                bool isDoorEdge = FP_IsDoorEdge(x, y, d, doorEdges);

                if (!isDoorEdge)
                    FP_SpawnWallOnEdge(origin, x, y, d, yBase, 0.3);

                FP_SpawnWallOnEdge(origin, x, y, d, yTop, 0.3);
            }
        }
    }
}

// WARPDOOR HELPER
static Object FP_SpawnWarpDoorOnEdge(vector origin, int gx, int gy, int dir /*0=N,1=E,2=S,3=W*/)
{
    float t = FP_TILE_SIZE;
    float y = origin[1] + FP_HEIGHT_OFFSET;

    float wx, wz, yaw;

    if (dir == 0)      { wx = origin[0] + gx * t;         wz = origin[2] + (gy - 0.5) * t; yaw = 0;   }
    else if (dir == 2) { wx = origin[0] + gx * t;         wz = origin[2] + (gy + 0.5) * t; yaw = 0;   }
    else if (dir == 1) { wx = origin[0] + (gx + 0.5) * t; wz = origin[2] + gy * t;        yaw = 90;  }
    else               { wx = origin[0] + (gx - 0.5) * t; wz = origin[2] + gy * t;        yaw = 90;  }

    vector pos = Vector(wx, y, wz);
    vector ori = Vector(yaw, 0, 0);
	
    // return the spawned door so we can read its memory points
    return SpawnDIBunkerObject("fmdpk_modscifiwalls_doorframe_warp", pos, ori, 1.0);
}

// ================= helpers to test if a world pos is inside the dungeon grid
static bool FP_WorldToGridCell(vector origin, float wx, float wz, int W, int H, out int gx, out int gy)
{
    float t = FP_TILE_SIZE;
    float fx = (wx - origin[0]) / t;
    float fz = (wz - origin[2]) / t;
    gx = Math.Floor(fx);
    gy = Math.Floor(fz);
    if (gx < 0 || gx >= W || gy < 0 || gy >= H) return false;
    return true;
}

static bool FP_IsWalkableAtWorld(vector origin, array<ref array<int>> grid, int W, int H, vector wpos)
{
    int gx, gy;
    if (!FP_WorldToGridCell(origin, wpos[0], wpos[2], W, H, gx, gy)) return false;
    return grid[gy][gx] != FP_EMPTY;
}

// Pick a perimeter edge (prefer halls) and return cell + outward dir
static bool FP_FindPerimeterEntrance(array<ref array<int>> grid, int W, int H, out int outGX, out int outGY, out int outDir)
{
    for (int pass = 0; pass < 2; pass++)
    {
        int want;
        if (pass == 0)
            want = FP_HALL;
        else
            want = FP_ROOM;

        // Top row (N)
        int y = 0;
        for (int x = 0; x < W; x++)
        {
            if (grid[y][x] == want)
            { outGX = x; outGY = y; outDir = 0; return true; }
        }
        // Right col (E)
        int xR = W - 1;
        for (int y2 = 0; y2 < H; y2++)
        {
            if (grid[y2][xR] == want)
            { outGX = xR; outGY = y2; outDir = 1; return true; }
        }
        // Bottom row (S)
        int yB = H - 1;
        for (int x2 = 0; x2 < W; x2++)
        {
            if (grid[yB][x2] == want)
            { outGX = x2; outGY = yB; outDir = 2; return true; }
        }
        // Left col (W)
        int xL = 0;
        for (int y3 = 0; y3 < H; y3++)
        {
            if (grid[y3][xL] == want)
            { outGX = xL; outGY = y3; outDir = 3; return true; }
        }
    }
    return false;
}

// CEILING HELPER
static void FP_SpawnCeilingAt(vector origin, int gx, int gy)
{
    float y = origin[1] + FP_HEIGHT_OFFSET + 6.0; // 6m above floor plane
    vector pos = Vector(origin[0] + gx * FP_TILE_SIZE, y, origin[2] + gy * FP_TILE_SIZE);
    SpawnDIBunkerObject("fmdpk_modscifiwalls_ceiling", pos, "0 0 0", 1.0);
}
