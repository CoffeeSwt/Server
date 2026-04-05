// HELPER FILES
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
void GenerateMap(vector origin)
{
    array<ref BaseLayout> layouts = new array<ref BaseLayout>;
    // layouts.Insert( new Layout_CustomGrid11x12 );
    //layouts.Insert( new Layout_CustomGrid20x19 );
    //layouts.Insert( new Layout_CustomGrid20x20 );
    //layouts.Insert( new Layout_CustomGrid22x27 );
    //layouts.Insert( new Layout_Plus12Rooms_25x25 );
    // layouts.Insert( new Layout_CustomGridCustom1 );
	// layouts.Insert( new Layout_CustomGrid41x41 );
	// layouts.Insert( new Layout_SmallBunker_1 );
	// layouts.Insert( new Layout_SmallBunker_2 );
	// layouts.Insert( new Layout_MediumBunker_1 );
    //layouts.Insert( new Layout_MediumBunker_2 );
    // layouts.Insert( new Layout_LargeBunker_1 );
    // layouts.Insert( new Layout_LargeBunker_2 );
    layouts.Insert( new Layout_2FloorMazeBunker );
    //layouts.Insert( new Layout_OpenAreaBunker );
    //layouts.Insert( new Layout_11x11_3xFullGearRooms );
    // layouts.Insert( new Layout_ModularTest );
	
    int choice = Math.RandomInt( 0, layouts.Count() );
	
	// Log which layout is being loaded.
	Print( "[BunkerGenerator] Spawning Layout ID: " + layouts[choice] );
	
    layouts[choice].Spawn( origin );
}

void CleanupSciFiWalls()
{
    array<Object> foundObjects = new array<Object>;
    GetGame().GetObjectsAtPosition( Vector(0, 0, 0 ), 999999, foundObjects, null );

    for ( int i = 0; i < foundObjects.Count(); i++ )
    {
        Object obj = foundObjects[i];
        if ( !obj ) continue;

        string typeName = obj.GetType();
        typeName.ToLower();

        if ( typeName.Contains( "fmdpk_modscifiwalls_" ) )
        {
            GetGame().ObjectDelete( obj );
        }
    }
}

// RANDOM BUNKER GENERATOR SPAWNER ===========================================
static Object SpawnDIBunkerObject(string type, vector position, vector orientation, float scale = 1.0)
{
    Object obj;
    if (type.Contains(".p3d")) {
        obj = GetGame().CreateStaticObjectUsingP3D(type, position, orientation, scale, false);
    } else {        
        obj = GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_CREATEPHYSICS | ECE_UPDATEPATHGRAPH | ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD);
        // obj = GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD | ECE_NOPERSISTENCY_CHAR);
    }

    if (!obj) {
        Error("Failed to create object " + type);
        return null;
    }

    obj.SetPosition(position);
    obj.SetOrientation(orientation);
    obj.SetOrientation(obj.GetOrientation());
    obj.SetScale(scale);
    obj.Update();
	obj.SetAffectPathgraph(true, false);
	if (obj.CanAffectPathgraph()) {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
    } 

    return obj;
}
class BaseLayout
{
    void Spawn(vector origin) {}
}
// ========================== BUNKER AI ZONE ==========================
class BunkerAIZone
{
	protected vector m_Center;
	protected float m_Radius;
	protected int m_WaveSize;
	protected int m_CooldownMs;
	protected bool m_Locked;                  // simple cooldown lock
	protected ref set<PlayerBase> m_Inside;   // who is currently inside

	void BunkerAIZone(vector center, float radius = 20.0, int waveSize = 10, int cooldownMs = 300000)
	{
		m_Center     = center;
		m_Radius     = radius;
		m_WaveSize   = FP_ZED_WAVE_SIZE;   // <-- ignore ctor arg; always use the one constant
		m_CooldownMs = cooldownMs;
		m_Inside     = new set<PlayerBase>;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Check, 1000, true);
	}

	void ~BunkerAIZone()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(Check);
	}

	protected void Check()
	{
		if (!GetGame().IsServer()) return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man m : players)
		{
			PlayerBase p = PlayerBase.Cast(m);
			if (!p) continue;

			bool inNow = vector.DistanceSq(p.GetPosition(), m_Center) <= (m_Radius * m_Radius);
			int idx = m_Inside.Find(p);          // index in the set (or -1 if not found)
			bool wasIn = idx != -1;

			if (inNow && !wasIn)
			{
				m_Inside.Insert(p);
				OnEnter(p);
			}
			else if (!inNow && wasIn)
			{
				m_Inside.Remove(idx);            // <-- Remove by index, not by object
				OnLeave(p);
			}
		}
	}

	protected void OnEnter(PlayerBase player)
	{
		if (m_Locked) return;
		if (FP_HaveInfectedWithin(m_Center, 300.0)) return;
		SpawnWaveAtRandomFloors(m_WaveSize);
		m_Locked = true;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Unlock, m_CooldownMs, false);
	}

	protected void OnLeave(PlayerBase player)
	{
		// optional: do something when leaving
	}

	protected void Unlock()
	{
		m_Locked = false;
	}

	protected void SpawnWave()
	{
		// Choose a few appropriate infected types (swap for your theme)
		ref array<string> pool = { "ZmbM_NBC_White", "ZmbM_NBC_Grey", "ZmbM_NBC_Yellow" };

		for (int i = 0; i < m_WaveSize; i++)
		{
			vector offset = Vector( Math.RandomFloat(-m_Radius * 0.7, m_Radius * 0.7), 0, Math.RandomFloat(-m_Radius * 0.7, m_Radius * 0.7) );

			vector pos = m_Center + offset;

			// put zeds on the ground; if your floors are navmeshed this is fine inside too
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);

			string type = pool.Get(Math.RandomInt(0, pool.Count()));
			// EntityAI ent = EntityAI.Cast(GetGame().CreateObjectEx(type, pos, ECE_INITAI | ECE_KEEPHEIGHT | ECE_NOLIFETIME | ECE_CREATEPHYSICS));
			EntityAI ent = EntityAI.Cast(GetGame().CreateObjectEx(type, pos, ECE_SETUP | ECE_INITAI | ECE_EQUIP_ATTACHMENTS | ECE_KEEPHEIGHT | ECE_NOSURFACEALIGN | ECE_NOLIFETIME | ECE_CREATEPHYSICS));

			DayZInfected zmb = DayZInfected.Cast(ent);
			if (zmb)
			{
				zmb.PlaceOnSurface();
				// optional: set patrol/target here
			}
		}
	}
}

// PRE-SET MODULES
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

// LAYOUTS
class Layout_CustomGrid20x20 : BaseLayout
{
	override void Spawn(vector origin)
	{
		const int W = 20;
		const int H = 20;

		// 1) Build grid (E by default)
		array<ref array<int>> grid;
		FP_GridInit(grid, W, H);

		// 2) Halls (use your map as reference)
		// Top horizontal hallway band (rows 1..2, cols 2..17)
		FP_GridFillRect(grid,  2,  1, 16, 2, FP_HALL);

		// Vertical hallway spines (cols 2..3 and 16..17 spanning rows 1..18)
		FP_GridFillRect(grid,  2,  1,  2, 18, FP_HALL);
		FP_GridFillRect(grid, 16,  1,  2, 18, FP_HALL);

		// 3) Rooms (each is 5×5), two columns × three rows
		// Top band
		FP_GridFillRect(grid,  4,  3, 5, 5, FP_ROOM);  // left
		FP_GridFillRect(grid, 11,  3, 5, 5, FP_ROOM);  // right

		// Middle band
		FP_GridFillRect(grid,  4,  9, 5, 5, FP_ROOM);  // left
		FP_GridFillRect(grid, 11,  9, 5, 5, FP_ROOM);  // right

		// Bottom band
		FP_GridFillRect(grid,  4, 15, 5, 5, FP_ROOM);  // left
		FP_GridFillRect(grid, 11, 15, 5, 5, FP_ROOM);  // right

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
				
				if (dir.Length() > 0.001)
					dir.Normalize();
				else
					dir = warpDoor.GetDirection();
				
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

		// 7) Walls: base layer + top layer (+3m), skipping base at doors
		FP_GenerateWalls(origin, grid, W, H, doorEdges);

		// 8) Ceilings: 6m above floor plane for every non-empty cell
		for (int cy = 0; cy < H; cy++)
		{
			for (int cx = 0; cx < W; cx++)
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}
		
		// 9) One container per room (centered on a floor tile's 'entity_spawn_point')
		FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
	}
}
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
class Layout_CustomGrid22x27 : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 22;
		const int H = 27;

		// 1) Build Grid
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

		// 2) HALLWAYS (FP_HALL)
		// Large lower-center hallway block
		FP_GridFillRect(grid,	0,	9,	22,	5,	FP_HALL);
		// FP_GridFillRect(grid,	0,	25,	22,	2,	FP_HALL);

		// Side hallway strips
		FP_GridFillRect(grid,	0,	5,	10,	4,	FP_HALL);
		FP_GridFillRect(grid,	10,	5,	12, 4,	FP_HALL);

		// Top hallway area
		FP_GridFillRect(grid, 0, 0, 10, 9, FP_HALL);
		FP_GridFillRect(grid, 10, 0, 12, 5, FP_HALL);

		// 3) ROOMS (FP_ROOM)
		// Upper left rooms
		FP_GridFillRect(grid, 1, 0, 9, 5, FP_ROOM);
		// Upper right rooms
		FP_GridFillRect(grid, 11, 0, 10, 5, FP_ROOM);

		// Middle rooms left
		FP_GridFillRect(grid, 1, 5, 9, 4, FP_ROOM);
		// Middle rooms right
		FP_GridFillRect(grid, 11, 5, 10, 4, FP_ROOM);

		// Lower-left rooms
		FP_GridFillRect(grid, 1, 14, 9, 6, FP_ROOM);
		// Lower-right rooms
		FP_GridFillRect(grid, 11, 14, 10, 6, FP_ROOM);

		// Bottom rooms below large hallway
		FP_GridFillRect(grid, 1, 20, 9, 5, FP_ROOM);
		FP_GridFillRect(grid, 11, 20, 10, 5, FP_ROOM);

		// 4) Spawn floors for R + H, record floor objects
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid(floorObjs, W, H);
		
		for ( int y = 0; y < H; y++ )
		{
			for ( int x = 0; x < W; x++ )
			{
				int cell = grid[y][x];
				
				if ( cell != FP_ROOM && cell != FP_HALL )
					continue;
				
				Object f = FP_SpawnFloorAt( origin, x, y );
				floorObjs[y][x] = f;
			}
		}
		g_FP_FloorObjs = floorObjs;

		// 5) Doors: one coded doorframe per contiguous room block
		array<ref array<bool>> visited = new array<ref array<bool>>;
		
		for ( int vy = 0; vy < H; vy++ )
		{
			auto vrow = new array<bool>;
			vrow.Resize( W );
			visited.Insert( vrow );
		}

		array<ref Param3<int,int,int>> doorEdges = new array<ref Param3<int,int,int>>;
		
		for ( int yy = 0; yy < H; yy++)
		{
			for ( int xx = 0; xx < W; xx++ )
			{
				if ( grid[yy][xx] == FP_ROOM && !visited[yy][xx] )
					FP_PlaceOneDoorForRoomRegion( grid, W, H, origin, xx, yy, visited, doorEdges );
			}
		}

		// 6) Entrance warp door on perimeter (prefers halls)
		int egx, egy, edir;
		
		if ( FP_FindPerimeterEntrance( grid, W, H, egx, egy, edir ) )
		{
			Object warpDoor = FP_SpawnWarpDoorOnEdge( origin, egx, egy, edir );
			FP_AddDoorEdge( doorEdges, egx, egy, edir );

			if ( warpDoor )
			{
				vector mpLocal = "0 0 0";
				
				if ( warpDoor.MemoryPointExists( "teleport_point" ) )
					mpLocal = warpDoor.GetMemoryPointPos( "teleport_point" );

				vector tpWorld = warpDoor.ModelToWorld( mpLocal );
				
				if ( !FP_IsWalkableAtWorld( origin, grid, W, H, tpWorld ) )
				{
					vector ori = warpDoor.GetOrientation();
					ori[0] = ori[0] + 180.0;
					warpDoor.SetOrientation(ori);
					tpWorld = warpDoor.ModelToWorld( mpLocal );
				}

				g_FP_WarpDoorTP_Pos = tpWorld;
				vector doorPos = warpDoor.GetPosition();
				vector dir = tpWorld - doorPos; dir[1] = 0.0;
				
				if ( dir.Length() > 0.001 ) dir.Normalize();
				else dir = warpDoor.GetDirection();
				
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

		// 7) Walls: base + top layers (+3m), skip base at door edges
		FP_GenerateWalls( origin, grid, W, H, doorEdges );

		// 8) Ceilings for every non-empty cell
		for ( int cy = 0; cy < H; cy++ )
		{
			for ( int cx = 0; cx < W; cx++ )
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}

		// 9) One container per contiguous room region
		FP_SpawnContainersForAllRooms( grid, W, H, origin, floorObjs, doorEdges );
		
		// 10) Extra Objects
		// Obsticles
		FP_SpawnObjectAtGrid(	origin,	0, 	5,	"fmdpk_modscifiwalls_doorframe_laser",		0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	0,	5,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		
		// Beds
		FP_SpawnObjectAtGrid(	origin,	4,	5,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	5,	5,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	6,	5,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		// Lootable Containers
		FP_SpawnMedicalAgainstWall( origin, 1, 5, 1, FP_MEDICAL_INSET, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
	}
}
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
class Layout_CustomGridCustom1 : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 25;
		const int H = 25;

		// 1) Build grid
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

		// 2) Halls
		FP_GridFillRect( grid,	9,	5,	2,	10,	FP_HALL );

		// 3) Rooms
		FP_GridFillRect( grid,	9,	15,	5,	5,	FP_ROOM );

		// 4) Spawn floors for R + H, record floor objects
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid( floorObjs, W, H );

		for ( int y = 0; y < H; y++ )
		{
			for ( int x = 0; x < W; x++ )
			{
				int cell = grid[y][x];
				
				if ( cell != FP_ROOM && cell != FP_HALL )
					continue;
				
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
			visited.Insert( vrow );
		}
		
		array<ref Param3<int,int,int>> doorEdges = new array<ref Param3<int,int,int>>;
		
		for (int yy = 0; yy < H; yy++)
		{
			for (int xx = 0; xx < W; xx++)
			{
				if ( grid[yy][xx] == FP_ROOM && !visited[yy][xx] )
					FP_PlaceOneDoorForRoomRegion( grid, W, H, origin, xx, yy, visited, doorEdges );
			}
		}

		// 6) Entrance warp door on perimeter (prefers halls)
		int egx, egy, edir;
		
		if (FP_FindPerimeterEntrance( grid, W, H, egx, egy, edir ) )
		{
			Object warpDoor = FP_SpawnWarpDoorOnEdge( origin, egx, egy, edir );
			FP_AddDoorEdge( doorEdges, egx, egy, edir );

			if ( warpDoor )
			{
				vector mpLocal = "0 0 0";
				
				if ( warpDoor.MemoryPointExists( "teleport_point" ) )
					mpLocal = warpDoor.GetMemoryPointPos( "teleport_point" );

				vector tpWorld = warpDoor.ModelToWorld( mpLocal );
				
				if ( !FP_IsWalkableAtWorld( origin, grid, W, H, tpWorld ) )
				{
					vector ori = warpDoor.GetOrientation();
					ori[0] = ori[0] + 180.0;
					warpDoor.SetOrientation(ori);
					tpWorld = warpDoor.ModelToWorld( mpLocal );
				}

				g_FP_WarpDoorTP_Pos = tpWorld;
				vector doorPos = warpDoor.GetPosition();
				vector dir = tpWorld - doorPos; dir[1] = 0.0;
				
				if ( dir.Length() > 0.001 )
					dir.Normalize();
				else
					dir = warpDoor.GetDirection();
				
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

		// 7) Walls: base layer + top layer (+3m), skipping base at doors
		FP_GenerateWalls( origin, grid, W, H, doorEdges );

		// 8) Ceilings: 6m above floor plane for every non-empty cell
		for ( int cy = 0; cy < H; cy++ )
		{
			for ( int cx = 0; cx < W; cx++ )
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt( origin, cx, cy );
			}
		}
		
		// 9) One container per room (centered on a floor tile's 'entity_spawn_point')
		FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
	}
}
class Layout_CustomGrid41x41 : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 41;
		const int H = 41;

// 1) Build grid (FP_EMPTY by default)
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

// 2) HALLWAYS ( FP_HALL )
		FP_GridFillRect( grid,	0,	0,	41,	2,	FP_HALL );
		FP_GridFillRect( grid,	0,	7,	41,	4,	FP_HALL );
		FP_GridFillRect( grid,	0,	23,	41,	4,	FP_HALL );
		FP_GridFillRect( grid,	0,	39,	41,	2,	FP_HALL );
		
		FP_GridFillRect( grid,	0,	0,	2,	41,	FP_HALL );
		FP_GridFillRect( grid,	19,	0,	2,	41,	FP_HALL );
		FP_GridFillRect( grid,	39,	0,	2,	41,	FP_HALL );

// 3) ROOMS ( FP_ROOM )
		FP_GridFillRect( grid,	2, 	2, 	5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	8, 	2, 	5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	14, 2, 	5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	21, 2, 	5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	27, 2, 	5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	33, 2, 	5, 	5, 	FP_ROOM );
		
		FP_GridFillRect( grid, 	2, 	11, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	8, 	11, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	14, 11, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	21, 11, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	27, 11, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	33, 11, 5, 	5, 	FP_ROOM );
		
		FP_GridFillRect( grid, 	2, 	18, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	8, 	18, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	14, 18, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	21, 18, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	27, 18, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	33, 18, 5, 	5, 	FP_ROOM );
		
		FP_GridFillRect( grid, 	2, 	27, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	8, 	27, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	14, 27, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	21, 27, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	27, 27, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	33, 27, 5, 	5, 	FP_ROOM );
		
		FP_GridFillRect( grid, 	2, 	33, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	8, 	33, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	14, 33, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	21, 33, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	27, 33, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	33, 33, 5, 	5, 	FP_ROOM );

// 4) Spawn floors for R + H, record floor objects
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid(floorObjs, W, H);
		
		for ( int y = 0; y < H; y++ )
		{
			for ( int x = 0; x < W; x++ )
			{
				int cell = grid[y][x];
				
				if ( cell != FP_ROOM && cell != FP_HALL )
					continue;
				
				Object f = FP_SpawnFloorAt( origin, x, y );
				floorObjs[y][x] = f;
			}
		}
		g_FP_FloorObjs = floorObjs;
		
		FP_SpawnObjectAtGrid( origin, 0, 0, "fmdpk_modscifiwalls_floor", 0.0 );

// 5) Doors: one coded doorframe per contiguous room block
		array<ref array<bool>> visited = new array<ref array<bool>>;
		
		for ( int vy = 0; vy < H; vy++ )
		{
			auto vrow = new array<bool>;
			vrow.Resize( W );
			visited.Insert( vrow );
		}

		array<ref Param3<int,int,int>> doorEdges = new array<ref Param3<int,int,int>>;
		
		for ( int yy = 0; yy < H; yy++)
		{
			for ( int xx = 0; xx < W; xx++ )
			{
				if ( grid[yy][xx] == FP_ROOM && !visited[yy][xx] )
					FP_PlaceOneDoorForRoomRegion( grid, W, H, origin, xx, yy, visited, doorEdges );
			}
		}

// 6) Entrance warp door on perimeter (prefers halls)
		int egx, egy, edir;
		
		if ( FP_FindPerimeterEntrance( grid, W, H, egx, egy, edir ) )
		{
			Object warpDoor = FP_SpawnWarpDoorOnEdge( origin, egx, egy, edir );
			FP_AddDoorEdge( doorEdges, egx, egy, edir );

			if ( warpDoor )
			{
				vector mpLocal = "0 0 0";
				
				if ( warpDoor.MemoryPointExists( "teleport_point" ) )
					mpLocal = warpDoor.GetMemoryPointPos( "teleport_point" );

				vector tpWorld = warpDoor.ModelToWorld( mpLocal );
				
				if ( !FP_IsWalkableAtWorld( origin, grid, W, H, tpWorld ) )
				{
					vector ori = warpDoor.GetOrientation();
					ori[0] = ori[0] + 180.0;
					warpDoor.SetOrientation(ori);
					tpWorld = warpDoor.ModelToWorld( mpLocal );
				}

				g_FP_WarpDoorTP_Pos = tpWorld;
				vector doorPos = warpDoor.GetPosition();
				vector dir = tpWorld - doorPos; dir[1] = 0.0;
				
				if ( dir.Length() > 0.001 ) dir.Normalize();
				else dir = warpDoor.GetDirection();
				
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

// 7) Walls: base + top layers (+3m), skip base at door edges
		FP_GenerateWalls( origin, grid, W, H, doorEdges );

// 8) Ceilings for every non-empty cell
		for ( int cy = 0; cy < H; cy++ )
		{
			for ( int cx = 0; cx < W; cx++ )
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}

// 9) One container per contiguous room region
		FP_SpawnContainersForAllRooms( grid, W, H, origin, floorObjs, doorEdges );
		
// 10 ) Cut Holes into Grid
		FP_DeleteFloorTile(		0, 		10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		0, 		19,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		1, 		19,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		0, 		20,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		1, 		20,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		0, 		21,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		1, 		21,	W,	H,	grid,	floorObjs );
		
// 11 ) Spawn Additional Parts
	// Spawn Obsticles
		FP_SpawnObjectAtGrid(	origin,	0, 	5,	"fmdpk_modscifiwalls_doorframe_laser",		0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	0,	5,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	1,	5,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	1,	5,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	0,	18,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	18,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	18,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	18,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	19,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	19,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	19,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	19,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
		// Entrance Wall Obsticles
		FP_SpawnObjectOnEdge(	origin,	1,	0,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	1,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	0,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	1,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	0,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	1,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	0,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	1,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		
		// First 2-Wide Hallway Wall Obsticles
		FP_SpawnObjectOnEdge(	origin,	31,	0,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	1,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	0,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	1,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	0,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	1,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	0,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	1,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		
		// Second 2-Wide Hallway Wall Obsticles
		FP_SpawnObjectOnEdge(	origin,	31,	39,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	40,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	39,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	40,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	39,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	40,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	39,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	40,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		
		// First 4-Wide Hallway Divider
		FP_SpawnObjectOnEdge(	origin,	20,	7,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	10,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	7,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	10,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	7,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	10,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	7,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	10,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	8,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	9,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	8,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	9,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	20, 8,	"fmdpk_modscifiwalls_doorframe_coded_wide",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	0 );
		
		// Second 4-Wide Hallway Divider
		FP_SpawnObjectOnEdge(	origin,	20,	23,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	26,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	23,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	26,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	26,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	26,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	24,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	25,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	25,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	20, 24,	"fmdpk_modscifiwalls_doorframe_coded_wide",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	0 );
		
		// 4-Wide Dividers
		FP_SpawnObjectOnEdge(	origin,	39,	17,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	17,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	17,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	17,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	16,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	16,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	16,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	16,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
		// First 4-Wide Section, Closets
		FP_SpawnObjectOnEdge(	origin,	39,	7,	"fmdpk_modscifiwalls_doorframe_coded",		0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	39,	10,	"fmdpk_modscifiwalls_doorframe_coded",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	39,	23,	"fmdpk_modscifiwalls_doorframe_coded",		0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	39,	26,	"fmdpk_modscifiwalls_doorframe_coded",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	40,	7,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	7,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	7,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	6,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	6,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	6,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	40,	11,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	11,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	11,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	10,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	10,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	10,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	40,	23,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	23,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	23,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	22,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	22,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	22,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	40,	27,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	27,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	27,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	26,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	26,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	26,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );

	// Spawn First 4-Wide Hall Beds
		FP_SpawnObjectOnEdge(	origin,	4,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	5,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	6,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	8,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	9,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	10,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	12,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	13,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	14,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	16,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	17,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	18,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );

	// Spawn Second 4-Wide Hall Beds
		FP_SpawnObjectOnEdge(	origin,	4,	24,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	5,	24,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	6,	24,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
	// Spawn Lootable Containers
		FP_SpawnObjectOnEdge(	origin,	2, 	5,	"fmdpk_modscifiwalls_medicalcab",			1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	1.2 );
		
	// Spawn Trap 1 and Exit Slope 1
		FP_SpawnObjectAtGrid(	origin,	0,	10,	"fmdpk_modscifiwalls_floor_trap",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	0,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	1,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	0,	11,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	1,	11,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	0,	12,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	1,	12,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	11,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	11,	"fmdpk_modscifiwalls_doorframe_laser",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectOnEdge(	origin,	0,	11,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	11,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	11,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	11,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	12,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	12,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	12,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	12,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	12,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	12,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	13,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	13,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	13,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	13,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	13,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	13,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	13,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	1,	13,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	14,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	14,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	14,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	14,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	14,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	14,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	14,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	1,	14,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	15,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1,		0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	15,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	15,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,		0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	15,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	15,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	15,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	15,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 2 );
		FP_SpawnObjectAtGrid(	origin,	1,	15,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 2 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	16,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	16,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	16,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	16,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	16,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	1,	16,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	17,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	17,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	17,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	17,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	17,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		FP_SpawnObjectAtGrid(	origin,	1,	17,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	18,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	18,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	18,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	18,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	18,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		FP_SpawnObjectAtGrid(	origin,	1,	18,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	19,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	19,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	19,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	1,	19,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	20,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	20,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	20,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	1,	20,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	21,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	21,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	21,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		FP_SpawnObjectAtGrid(	origin,	1,	21,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		
		//Exit Doorframe
		FP_SpawnObjectOnEdge(	origin,	1, 	23,	"fmdpk_modscifiwalls_doorframe_laser_wide",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	23,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	23,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	22,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	22,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
	// Spawn Security Room
		// Spawn Security Room Floors
		FP_SpawnObjectAtGrid(	origin,	2,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	11,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	12,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	11,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	12,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		// Spawn Security Room Walls
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		//
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	11,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	3,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	3,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	11,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	11,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	3,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	3,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		
		// Spawn Security Room Ceilings
		FP_SpawnObjectAtGrid(	origin,	2,	10,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	11,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	12,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	10,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	11,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	12,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
	}
}
class Layout_SmallBunker_1 : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 7;
		const int H = 9;

// 1) Build Grid
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

// 2) HALLWAYS ( FP_HALL )
		FP_GridFillRect(	grid,	3,	0,	1,	9,	FP_HALL );

// 3) ROOMS ( FP_ROOM )
		FP_GridFillRect(	grid,	0,	0, 	3,	3,	FP_ROOM );
		FP_GridFillRect(	grid, 	4,	0,	3, 	3, 	FP_ROOM );
		FP_GridFillRect(	grid, 	0,	6,	3, 	3, 	FP_ROOM );
		FP_GridFillRect(	grid, 	4,	6,	3, 	3, 	FP_ROOM );

// 4) Spawn floors for R + H, record floor objects
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid(floorObjs, W, H);
		
		for ( int y = 0; y < H; y++ )
		{
			for ( int x = 0; x < W; x++ )
			{
				int cell = grid[y][x];
				
				if ( cell != FP_ROOM && cell != FP_HALL )
					continue;
				
				Object f = FP_SpawnFloorAt( origin, x, y );
				floorObjs[y][x] = f;
			}
		}
		g_FP_FloorObjs = floorObjs;

// 5) Doors: one coded doorframe per contiguous room block
		array<ref array<bool>> visited = new array<ref array<bool>>;
		
		for ( int vy = 0; vy < H; vy++ )
		{
			auto vrow = new array<bool>;
			vrow.Resize( W );
			visited.Insert( vrow );
		}

		array<ref Param3<int,int,int>> doorEdges = new array<ref Param3<int,int,int>>;
		
		for ( int yy = 0; yy < H; yy++)
		{
			for ( int xx = 0; xx < W; xx++ )
			{
				if ( grid[yy][xx] == FP_ROOM && !visited[yy][xx] )
					FP_PlaceOneDoorForRoomRegion( grid, W, H, origin, xx, yy, visited, doorEdges );
			}
		}

// 6) Entrance warp door on perimeter (prefers halls)
		int egx, egy, edir;
		
		if ( FP_FindPerimeterEntrance( grid, W, H, egx, egy, edir ) )
		{
			Object warpDoor = FP_SpawnWarpDoorOnEdge( origin, egx, egy, edir );
			FP_AddDoorEdge( doorEdges, egx, egy, edir );

			if ( warpDoor )
			{
				vector mpLocal = "0 0 0";
				
				if ( warpDoor.MemoryPointExists( "teleport_point" ) )
					mpLocal = warpDoor.GetMemoryPointPos( "teleport_point" );

				vector tpWorld = warpDoor.ModelToWorld( mpLocal );
				
				if ( !FP_IsWalkableAtWorld( origin, grid, W, H, tpWorld ) )
				{
					vector ori = warpDoor.GetOrientation();
					ori[0] = ori[0] + 180.0;
					warpDoor.SetOrientation(ori);
					tpWorld = warpDoor.ModelToWorld( mpLocal );
				}

				g_FP_WarpDoorTP_Pos = tpWorld;
				vector doorPos = warpDoor.GetPosition();
				vector dir = tpWorld - doorPos; dir[1] = 0.0;
				
				if ( dir.Length() > 0.001 ) dir.Normalize();
				else dir = warpDoor.GetDirection();
				
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

// 7) Walls: base + top layers (+3m), skip base at door edges
		FP_GenerateWalls( origin, grid, W, H, doorEdges );

// 8) Ceilings for every non-empty cell
		for ( int cy = 0; cy < H; cy++ )
		{
			for ( int cx = 0; cx < W; cx++ )
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}
		
// 9) One container per contiguous room region
		// Use this if not defining your own > FP_SpawnContainersForAllRooms( grid, W, H, origin, floorObjs, doorEdges );
		FP_SpawnObjectAtGrid( origin, 1, 1, "fmdpk_modscifiwalls_container", 180.0, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid( origin, 5, 1, "fmdpk_modscifiwalls_container", 180.0, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );

		FP_SpawnObjectAtGrid( origin, 1, 7, "fmdpk_modscifiwalls_container", 0.0, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid( origin, 5, 7, "fmdpk_modscifiwalls_container", 0.0, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
// 10 ) Spawn Additional Parts
		
	// Spawn Obsticles
		FP_SpawnObjectAtGrid( origin, 3, 4, "fmdpk_modscifiwalls_doorframe_laser", 0.0, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid( origin, 3, 4, "fmdpk_modscifiwalls_halfwall_full", 0.0, FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		
	// Spawn Medical Containers
		FP_SpawnMedicalAgainstWall( origin, 1, 8, 2, FP_MEDICAL_INSET, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnMedicalAgainstWall( origin, 5, 8, 2, FP_MEDICAL_INSET, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
	// Spawn Rifle Containers
		FP_SpawnLockerAgainstWall( origin, 1, 0, 0, FP_LOCKER_INSET, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnLockerAgainstWall( origin, 5, 0, 0, FP_LOCKER_INSET, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
	}
}
class Layout_SmallBunker_2 : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 9;
		const int H = 9;

// 1) Build Grid
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

// 2) HALLWAYS ( FP_HALL )
		FP_GridFillRect(	grid,	0,	0,	9,	9,	FP_HALL );

// 3) ROOMS ( FP_ROOM )
		FP_GridFillRect(	grid,	1,	1, 	3,	3,	FP_ROOM );
		FP_GridFillRect(	grid, 	1,	5,	3, 	3, 	FP_ROOM );
		FP_GridFillRect(	grid, 	5,	1,	3, 	3, 	FP_ROOM );
		FP_GridFillRect(	grid, 	5,	5,	3, 	3, 	FP_ROOM );

// 4) Spawn floors for R + H, record floor objects
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid(floorObjs, W, H);
		
		for ( int y = 0; y < H; y++ )
		{
			for ( int x = 0; x < W; x++ )
			{
				int cell = grid[y][x];
				
				if ( cell != FP_ROOM && cell != FP_HALL )
					continue;
				
				Object f = FP_SpawnFloorAt( origin, x, y );
				floorObjs[y][x] = f;
			}
		}
		g_FP_FloorObjs = floorObjs;

// 5) Doors: one coded doorframe per contiguous room block
		array<ref array<bool>> visited = new array<ref array<bool>>;
		
		for ( int vy = 0; vy < H; vy++ )
		{
			auto vrow = new array<bool>;
			vrow.Resize( W );
			visited.Insert( vrow );
		}

		array<ref Param3<int,int,int>> doorEdges = new array<ref Param3<int,int,int>>;
		
		for ( int yy = 0; yy < H; yy++)
		{
			for ( int xx = 0; xx < W; xx++ )
			{
				if ( grid[yy][xx] == FP_ROOM && !visited[yy][xx] )
					FP_PlaceOneDoorForRoomRegion( grid, W, H, origin, xx, yy, visited, doorEdges );
			}
		}

// 6) Entrance warp door on perimeter (prefers halls)
		int egx, egy, edir;
		
		if ( FP_FindPerimeterEntrance( grid, W, H, egx, egy, edir ) )
		{
			Object warpDoor = FP_SpawnWarpDoorOnEdge( origin, egx, egy, edir );
			FP_AddDoorEdge( doorEdges, egx, egy, edir );

			if ( warpDoor )
			{
				vector mpLocal = "0 0 0";
				
				if ( warpDoor.MemoryPointExists( "teleport_point" ) )
					mpLocal = warpDoor.GetMemoryPointPos( "teleport_point" );

				vector tpWorld = warpDoor.ModelToWorld( mpLocal );
				
				if ( !FP_IsWalkableAtWorld( origin, grid, W, H, tpWorld ) )
				{
					vector ori = warpDoor.GetOrientation();
					ori[0] = ori[0] + 180.0;
					warpDoor.SetOrientation(ori);
					tpWorld = warpDoor.ModelToWorld( mpLocal );
				}

				g_FP_WarpDoorTP_Pos = tpWorld;
				vector doorPos = warpDoor.GetPosition();
				vector dir = tpWorld - doorPos; dir[1] = 0.0;
				
				if ( dir.Length() > 0.001 ) dir.Normalize();
				else dir = warpDoor.GetDirection();
				
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

// 7) Walls: base + top layers (+3m), skip base at door edges
		FP_GenerateWalls( origin, grid, W, H, doorEdges );

// 8) Ceilings for every non-empty cell
		for ( int cy = 0; cy < H; cy++ )
		{
			for ( int cx = 0; cx < W; cx++ )
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}
		
// 9) One container per contiguous room region
		// Use this if not defining your own > FP_SpawnContainersForAllRooms( grid, W, H, origin, floorObjs, doorEdges );
		FP_SpawnObjectAtGrid( origin, 2, 2, "fmdpk_modscifiwalls_container", 180.0, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid( origin, 6, 2, "fmdpk_modscifiwalls_container", 180.0, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid( origin, 2, 6, "fmdpk_modscifiwalls_container", 0.0, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid( origin, 6, 6, "fmdpk_modscifiwalls_container", 0.0, FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
// 10 ) Spawn Additional Parts
		
	// Spawn Obsticles
		FP_SpawnObjectOnEdge( origin, 5, 0, "fmdpk_modscifiwalls_doorframe_laser", 3.0, FIRST_FLOOR_LOWER_HEIGHT_OFFSET, -0.3 );
		FP_SpawnObjectOnEdge( origin, 5, 0, "fmdpk_modscifiwalls_halfwall_full", 3.0, FIRST_FLOOR_UPPER_HEIGHT_OFFSET, -0.3 );
		FP_SpawnObjectOnEdge( origin, 5, 8, "fmdpk_modscifiwalls_doorframe_laser", 3.0, FIRST_FLOOR_LOWER_HEIGHT_OFFSET, -0.3 );
		FP_SpawnObjectOnEdge( origin, 5, 8, "fmdpk_modscifiwalls_halfwall_full", 3.0, FIRST_FLOOR_UPPER_HEIGHT_OFFSET, -0.3 );
	}
}
class Layout_MediumBunker_1 : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 12;
		const int H = 13;

// 1) Build Grid
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

// 2) HALLWAYS ( FP_HALL )
		FP_GridFillRect(	grid,	5,	0,	2,	13,	FP_HALL );
		FP_GridFillRect(	grid,	0,	6,	12,	1,	FP_HALL );

// 3) ROOMS ( FP_ROOM )
		FP_GridFillRect(	grid,	0,	0, 	5,	5,	FP_ROOM );
		FP_GridFillRect(	grid, 	7,	0,	5, 	5, 	FP_ROOM );
		FP_GridFillRect(	grid, 	0,	8,	5, 	5, 	FP_ROOM );
		FP_GridFillRect(	grid, 	7,	8,	5, 	5, 	FP_ROOM );

// 4) Spawn floors for R + H, record floor objects
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid(floorObjs, W, H);
		
		for ( int y = 0; y < H; y++ )
		{
			for ( int x = 0; x < W; x++ )
			{
				int cell = grid[y][x];
				
				if ( cell != FP_ROOM && cell != FP_HALL )
					continue;
				
				Object f = FP_SpawnFloorAt( origin, x, y );
				floorObjs[y][x] = f;
			}
		}
		g_FP_FloorObjs = floorObjs;

// 5) Doors: one coded doorframe per contiguous room block
		array<ref array<bool>> visited = new array<ref array<bool>>;
		
		for ( int vy = 0; vy < H; vy++ )
		{
			auto vrow = new array<bool>;
			vrow.Resize( W );
			visited.Insert( vrow );
		}

		array<ref Param3<int,int,int>> doorEdges = new array<ref Param3<int,int,int>>;
		
		for ( int yy = 0; yy < H; yy++)
		{
			for ( int xx = 0; xx < W; xx++ )
			{
				if ( grid[yy][xx] == FP_ROOM && !visited[yy][xx] )
					FP_PlaceOneDoorForRoomRegion( grid, W, H, origin, xx, yy, visited, doorEdges );
			}
		}

// 6) Entrance warp door on perimeter (prefers halls)
		int egx, egy, edir;
		
		if ( FP_FindPerimeterEntrance( grid, W, H, egx, egy, edir ) )
		{
			Object warpDoor = FP_SpawnWarpDoorOnEdge( origin, egx, egy, edir );
			FP_AddDoorEdge( doorEdges, egx, egy, edir );

			if ( warpDoor )
			{
				vector mpLocal = "0 0 0";
				
				if ( warpDoor.MemoryPointExists( "teleport_point" ) )
					mpLocal = warpDoor.GetMemoryPointPos( "teleport_point" );

				vector tpWorld = warpDoor.ModelToWorld( mpLocal );
				
				if ( !FP_IsWalkableAtWorld( origin, grid, W, H, tpWorld ) )
				{
					vector ori = warpDoor.GetOrientation();
					ori[0] = ori[0] + 180.0;
					warpDoor.SetOrientation(ori);
					tpWorld = warpDoor.ModelToWorld( mpLocal );
				}

				g_FP_WarpDoorTP_Pos = tpWorld;
				vector doorPos = warpDoor.GetPosition();
				vector dir = tpWorld - doorPos; dir[1] = 0.0;
				
				if ( dir.Length() > 0.001 ) dir.Normalize();
				else dir = warpDoor.GetDirection();
				
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

// 7) Walls: base + top layers (+3m), skip base at door edges
		FP_GenerateWalls( origin, grid, W, H, doorEdges );

// 8) Ceilings for every non-empty cell
		for ( int cy = 0; cy < H; cy++ )
		{
			for ( int cx = 0; cx < W; cx++ )
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}

// 9) One container per contiguous room region
		FP_SpawnContainersForAllRooms( grid, W, H, origin, floorObjs, doorEdges );

// 10 ) Cut Holes into Grid
		FP_DeleteFloorTile( 0, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 1, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 2, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 3, 6, W, H, grid, floorObjs );
		
		FP_DeleteFloorTile( 8, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 9, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 10, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 11, 6, W, H, grid, floorObjs );
		
		// FP_DeleteFloorTile( 1, 3, W, H, grid, floorObjs );
		// FP_DeleteFloorTile( 10, 3, W, H, grid, floorObjs );
		// FP_DeleteFloorTile( 1, 9, W, H, grid, floorObjs );
		// FP_DeleteFloorTile( 10, 9, W, H, grid, floorObjs );

// 11 ) Spawn Additional Parts
	// Spawn Obsticles
		FP_SpawnObjectAtGrid(	origin,	6, 	5,	"fmdpk_modscifiwalls_doorframe_coded_wide",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	6,	5,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	6, 	7,	"fmdpk_modscifiwalls_doorframe_coded_wide",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	5,	7,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	6,	7,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		
		// FP_SpawnObjectAtGrid(	origin,	1,	3,	"fmdpk_modscifiwalls_floor_trap",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		// FP_SpawnObjectAtGrid(	origin,	10,	3,	"fmdpk_modscifiwalls_floor_trap",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		// FP_SpawnObjectAtGrid(	origin,	1,	9,	"fmdpk_modscifiwalls_floor_trap",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		// FP_SpawnObjectAtGrid(	origin,	10,	9,	"fmdpk_modscifiwalls_floor_trap",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	0,	5,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	0,	7,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	11,	5,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	11,	7,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	0,	6,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	6,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	0,	5,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	4,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	3,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	2,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	1,	2,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	11,	5,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	4,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	3,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	2,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	10,	2,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	0,	7,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	8,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	9,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	1,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	11,	7,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	8,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	9,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	10,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	10,	6,	"fmdpk_modscifiwalls_floor_slope",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	9,	6,	"fmdpk_modscifiwalls_floor_slope",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	8,	6,	"fmdpk_modscifiwalls_floor_slope",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		
		FP_SpawnObjectAtGrid(	origin,	1,	6,	"fmdpk_modscifiwalls_floor_slope",			180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	6,	"fmdpk_modscifiwalls_floor_slope",			180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	3,	6,	"fmdpk_modscifiwalls_floor_slope",			180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		//
		FP_SpawnObjectOnEdge(	origin,	1,	2,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	1,	2,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	1,	2,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	2,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	2,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	3,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	3,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	4,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	4,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	5,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	5,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	10,	2,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	10,	2,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	10,	2,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	2,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	2,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	3,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	3,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	4,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	4,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	5,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	5,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	9,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	9,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	8,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	8,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	7,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	7,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	10,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	10,	10,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	10,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	10,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	9,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	9,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	8,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	8,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	7,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	7,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	6,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	6,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	1,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	1,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	2,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	2,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	3,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	3,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	4,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	4,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	7,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	7,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	8,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	8,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	9,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	9,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	10,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	10,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
	}
}
class Layout_MediumBunker_2 : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 15;
		const int H = 15;

// 1) Build Grid
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

// 2) HALLWAYS ( FP_HALL )
		FP_GridFillRect(	grid,	0,	0,	15,	2,	FP_HALL );
		FP_GridFillRect(	grid,	0,	0,	2,	15,	FP_HALL );
		FP_GridFillRect(	grid,	13,	0,	2,	15,	FP_HALL );
		FP_GridFillRect(	grid,	0,	13,	15,	2,	FP_HALL );

// 3) ROOMS ( FP_ROOM )
		FP_GridFillRect(	grid,	2,	2, 	5,	5,	FP_ROOM );
		FP_GridFillRect(	grid, 	8,	2,	5, 	5, 	FP_ROOM );
		FP_GridFillRect(	grid, 	2,	8,	5, 	5, 	FP_ROOM );
		FP_GridFillRect(	grid, 	8,	8,	5, 	5, 	FP_ROOM );

// 4) Spawn floors for R + H, record floor objects
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid(floorObjs, W, H);
		
		for ( int y = 0; y < H; y++ )
		{
			for ( int x = 0; x < W; x++ )
			{
				int cell = grid[y][x];
				
				if ( cell != FP_ROOM && cell != FP_HALL )
					continue;
				
				Object f = FP_SpawnFloorAt( origin, x, y );
				floorObjs[y][x] = f;
			}
		}
		g_FP_FloorObjs = floorObjs;

// 5) Doors: one coded doorframe per contiguous room block
		array<ref array<bool>> visited = new array<ref array<bool>>;
		
		for ( int vy = 0; vy < H; vy++ )
		{
			auto vrow = new array<bool>;
			vrow.Resize( W );
			visited.Insert( vrow );
		}

		array<ref Param3<int,int,int>> doorEdges = new array<ref Param3<int,int,int>>;
		
		for ( int yy = 0; yy < H; yy++)
		{
			for ( int xx = 0; xx < W; xx++ )
			{
				if ( grid[yy][xx] == FP_ROOM && !visited[yy][xx] )
					FP_PlaceOneDoorForRoomRegion( grid, W, H, origin, xx, yy, visited, doorEdges );
			}
		}

// 6) Entrance warp door on perimeter (prefers halls)
		int egx, egy, edir;
		
		if ( FP_FindPerimeterEntrance( grid, W, H, egx, egy, edir ) )
		{
			Object warpDoor = FP_SpawnWarpDoorOnEdge( origin, egx, egy, edir );
			FP_AddDoorEdge( doorEdges, egx, egy, edir );

			if ( warpDoor )
			{
				vector mpLocal = "0 0 0";
				
				if ( warpDoor.MemoryPointExists( "teleport_point" ) )
					mpLocal = warpDoor.GetMemoryPointPos( "teleport_point" );

				vector tpWorld = warpDoor.ModelToWorld( mpLocal );
				
				if ( !FP_IsWalkableAtWorld( origin, grid, W, H, tpWorld ) )
				{
					vector ori = warpDoor.GetOrientation();
					ori[0] = ori[0] + 180.0;
					warpDoor.SetOrientation(ori);
					tpWorld = warpDoor.ModelToWorld( mpLocal );
				}

				g_FP_WarpDoorTP_Pos = tpWorld;
				vector doorPos = warpDoor.GetPosition();
				vector dir = tpWorld - doorPos; dir[1] = 0.0;
				
				if ( dir.Length() > 0.001 ) dir.Normalize();
				else dir = warpDoor.GetDirection();
				
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

// 7) Walls: base + top layers (+3m), skip base at door edges
		FP_GenerateWalls( origin, grid, W, H, doorEdges );

// 8) Ceilings for every non-empty cell
		for ( int cy = 0; cy < H; cy++ )
		{
			for ( int cx = 0; cx < W; cx++ )
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}

// 9) One container per contiguous room region
		FP_SpawnContainersForAllRooms( grid, W, H, origin, floorObjs, doorEdges );

// 10 ) Spawn Additional Parts
	// Spawn Obsticles
		
		FP_SpawnObjectAtGrid(	origin,	6, 	0,	"fmdpk_modscifiwalls_doorframe_laser_wide",	90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	6,	0,	"fmdpk_modscifiwalls_halfwall_full",		90.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	6,	1,	"fmdpk_modscifiwalls_halfwall_full",		90.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	10, 13,	"fmdpk_modscifiwalls_doorframe_coded_wide",	90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	10,	13,	"fmdpk_modscifiwalls_halfwall_full",		90.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	10,	14,	"fmdpk_modscifiwalls_halfwall_full",		90.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
	}
}
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
class Layout_2FloorMazeBunker : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 29;
		const int H = 29;

		// 1) BUILD GRID
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

		// 2) DEFINE HALLWAYS
		FP_GridFillRect( grid,	6,	3,	3,	24,	FP_HALL );
		FP_GridFillRect( grid,	20,	3,	3,	24,	FP_HALL );
		FP_GridFillRect( grid,	9,	6,	4,	3,	FP_HALL );
		FP_GridFillRect( grid,	16,	6,	4,	3,	FP_HALL );

		// 3) DEFINE ROOMS
		FP_GridFillRect( grid,	1,	0,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	1,	6,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	1,	16,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	9,	1,	5,	5,	FP_ROOM );
		
		FP_GridFillRect( grid,	23,	0,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	23,	6,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	23,	12,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	15,	1,	5,	5,	FP_ROOM );

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
			FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
		
		// 10) REMOVE FLOORS FOR RAMPS
			DeleteDIBunkerFloorTiles( W, H, grid, floorObjs );
				
		// 11) ADD FIRST FLOOR ENTRANCES TO BASEMENT
			AddDIBunkerFirstFloorEntrances( origin );

		// 12) ADD BASEMENT FLOORS
			SpawnDIBunkerBasementFloors( origin );

		// 13) ADD BASEMENT WALLS AND DOORS
			SpawnDIBunkerBasementWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
			SpawnDIBunkerBasementWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
			
			SpawnDIBunkerEntranceF1Walls( origin, FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 0.0 );
			SpawnDIBunkerEntranceF1Walls( origin, FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 0.0 );
			
			SpawnDIBunkerBasementEntranceWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
			SpawnDIBunkerBasementEntranceWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.0 );
			SpawnDIBunkerBasementEntranceL3Walls( origin, 0.0 );
			
			SpawnDIBunkerBasementRoomWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
			SpawnDIBunkerBasementRoomWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
			
			SpawnDIBunkerBasementVentWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, -0.3 );
			
			SpawnDIBunkerGreebles( origin );
			
		// 14) ADD BASEMENT WALLS AND DOORS
			SpawnDIBunkerBasementRoomDoors( origin );
			
		// 15) ADD BASEMENT CEILINGS
			SpawnDIBunkerBasementCeilings( origin );
			SpawnDIBunkerBasementVentCeilings( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 - 0.4 );
			
		// 16) CREATE VERTICAL TRAPS
			SpawnDIBunkerVerticalShafts( origin );
			
		// 17) ADD LOOT TO BASEMENTS
			SpawnDIBunkerBasementLoot( origin );
	}

	void DeleteDIBunkerFloorTiles( int W, int H, array<ref array<int>> grid, array<ref array<Object>> floorObjs )
	{
		FP_DeleteFloorTile( 7,	24,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	23,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	22,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	21,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	20,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	19,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	16,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile( 21,	24,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 21,	23,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 21,	22,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 21,	21,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 21,	20,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 21,	19,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	21,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	21,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	21,	16,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	7,	8,	W,	H,	grid,	floorObjs );
	}
	
	void SpawnDIBunkerBasementLoot( vector origin)
	{
		FP_SpawnLockerAgainstWall( origin,	2,	5,	1.0, FP_LOCKER_INSET,		FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnLockerAgainstWall( origin,	3,	12,	2.0, FP_LOCKER_INSET,		FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnConatinerAgainstWall( origin,	25,	4,	2.0, FP_CONTAINER_INSET, 	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnLockerAgainstWall( origin,	26,	11,	3.0, FP_LOCKER_INSET, 		FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnConatinerAgainstWall( origin,	25,	18,	0.0, FP_CONTAINER_INSET, 	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnMedicalAgainstWall( origin,	3,	16,	0.0, FP_MEDICAL_INSET, 		FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
	}
	
	void SpawnDIBunkerVerticalShafts( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	7,	8,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.0 );
		FP_SpawnObjectOnEdge(	origin,	7,	8,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.0 );
		FP_SpawnObjectOnEdge(	origin,	7,	8,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.0 );
		FP_SpawnObjectOnEdge(	origin,	7,	8,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.0 );
		
		FP_SpawnObjectAtGrid(	origin,	7,	8,	"fmdpk_modscifiwalls_floor_trap_2",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	7,	8,	"fmdpk_modscifiwalls_floor_trap_2",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
	}
	
	void AddDIBunkerFirstFloorEntrances( vector origin )
	{
		int randomBasementEnt1 = Math.RandomInt(0,2);
		int randomBasementEnt2 = Math.RandomInt(0,2);
		
		if ( randomBasementEnt1 )
			FP_SpawnObjectOnEdge(	origin,	7,	15,	"fmdpk_modscifiwalls_doorframe_coded",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		else
			FP_SpawnObjectOnEdge(	origin,	7,	15,	"fmdpk_modscifiwalls_doorframe_laser",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		
		if ( randomBasementEnt2 )
			FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_doorframe_coded",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		else
			FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_doorframe_laser",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	7,	15,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
	}
	
	void SpawnDIBunkerBasementFloors( vector origin )
	{
			// SPAWN LEFT HALLWAY FLOORS
		FP_SpawnObjectAtGrid(	origin,	6,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN RIGHT HALLWAY FLOORS
		FP_SpawnObjectAtGrid(	origin,	20,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN SIDE ROOM HALLWAY FLOORS
		FP_SpawnObjectAtGrid(	origin,	5,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	5,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	5,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	23,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	23,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	23,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN LEFT SIDEROOM FLOORS
		FP_SpawnObjectAtGrid(	origin,	2,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectAtGrid(	origin,	2,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectAtGrid(	origin,	2,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN RIGHT SIDEROOM FLOORS
		FP_SpawnObjectAtGrid(	origin,	24,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectAtGrid(	origin,	24,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin, 26,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectAtGrid(	origin,	24,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN VENT ENTRANCE SLOPED FLOORS
		FP_SpawnObjectAtGrid(	origin,	9,	3,	"fmdpk_modscifiwalls_floor_slope",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	9,	9,	"fmdpk_modscifiwalls_floor_slope",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	9,	24,	"fmdpk_modscifiwalls_floor_slope",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	19,	3,	"fmdpk_modscifiwalls_floor_slope",		180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	19,	9,	"fmdpk_modscifiwalls_floor_slope",		180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	19,	24,	"fmdpk_modscifiwalls_floor_slope",		180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN BASEMENT SLOPED FLOORS
		FP_SpawnObjectAtGrid(	origin,	7,	24,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	24,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	23,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	23,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	7,	22,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 2 );
		FP_SpawnObjectAtGrid(	origin,	21,	22,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 2 );
		FP_SpawnObjectAtGrid(	origin,	7,	21,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	21,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	7,	20,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	20,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		FP_SpawnObjectAtGrid(	origin,	7,	19,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		FP_SpawnObjectAtGrid(	origin,	21,	19,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		FP_SpawnObjectAtGrid(	origin,	7,	18,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	18,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	17,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	17,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	7,	16,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		FP_SpawnObjectAtGrid(	origin,	21,	16,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		
			// SPAWN VENTING FLOORS
		FP_SpawnObjectAtGrid(	origin,	13,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	11,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	13,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	15,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	11,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	15,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	11,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	11,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	11,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	12,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	13,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	11,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	12,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	12,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	11,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	15,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	13,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	15,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	11,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	12,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	11,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	14,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	11,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	13,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	15,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
	}
	
	void SpawnDIBunkerGreebles( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	9,	6,	"fmdpk_modscifiwalls_static_wetfloorsign",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	9,	8,	"fmdpk_modscifiwalls_static_wetfloorsign",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	19,	6,	"fmdpk_modscifiwalls_static_wetfloorsign",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	19,	8,	"fmdpk_modscifiwalls_static_wetfloorsign",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );

		FP_SpawnObjectAtGrid(	origin,	7,	10,	"fmdpk_modscifiwalls_static_medicalbed",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	17,	7,	"fmdpk_modscifiwalls_static_medicalbed",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	8,	11,	"fmdpk_modscifiwalls_static_greeble_1",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GREEBLE_1_INSET );
		FP_SpawnObjectOnEdge(	origin,	8,	16,	"fmdpk_modscifiwalls_static_greeble_2",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GREEBLE_1_INSET );
		
		FP_SpawnObjectOnEdge(	origin,	20,	9,	"fmdpk_modscifiwalls_static_greeble_1",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GREEBLE_2_INSET );
		FP_SpawnObjectOnEdge(	origin,	20,	17,	"fmdpk_modscifiwalls_static_greeble_2",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GREEBLE_2_INSET );
	}
	
	void SpawnDIBunkerBasementRoomWalls( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, float hoffset = 0.3 )
	{
		FP_SpawnObjectOnEdge(	origin,	2,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	4,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	6,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	4,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	5,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	6,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
				// 2
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
				// 3
		FP_SpawnObjectOnEdge(	origin,	2,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		
			// SPAWN BASEMENT RIGHT ROOM WALLS
				// 1
		FP_SpawnObjectOnEdge(	origin,	24,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	4,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	5,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	6,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	4,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	6,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
				// 2
		FP_SpawnObjectOnEdge(	origin,	24,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
				// 3
		FP_SpawnObjectOnEdge(	origin,	24,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
	}
	
	void SpawnDIBunkerBasementRoomDoors( vector origin )
	{
		int randomBasementDoor1 = Math.RandomInt(0,2);
		int randomBasementDoor2 = Math.RandomInt(0,2);
		int randomBasementDoor3 = Math.RandomInt(0,2);
		int randomBasementDoor4 = Math.RandomInt(0,2);
		int randomBasementDoor5 = Math.RandomInt(0,2);
		int randomBasementDoor6 = Math.RandomInt(0,2);
		
		if ( randomBasementDoor1 )
			FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_doorframe_laser",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		if ( randomBasementDoor2 )
			FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_doorframe_laser",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		if ( randomBasementDoor3 )
			FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_doorframe_laser",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		if ( randomBasementDoor4 )
			FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_doorframe_coded",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_doorframe_laser",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		if ( randomBasementDoor5 )
			FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_doorframe_coded",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_doorframe_laser",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		if ( randomBasementDoor6 )
			FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_doorframe_coded",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_doorframe_laser",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
			// SPAWN VENT HATCHES
		FP_SpawnObjectOnEdge(	origin,	10,	3,	"fmdpk_modscifiwalls_doorframe_vent",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		FP_SpawnObjectOnEdge(	origin,	10,	9,	"fmdpk_modscifiwalls_doorframe_vent",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		FP_SpawnObjectOnEdge(	origin,	10,	24,	"fmdpk_modscifiwalls_doorframe_vent",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	3,	"fmdpk_modscifiwalls_doorframe_vent",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		FP_SpawnObjectOnEdge(	origin,	18,	9,	"fmdpk_modscifiwalls_doorframe_vent",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		FP_SpawnObjectOnEdge(	origin,	18,	24,	"fmdpk_modscifiwalls_doorframe_vent",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
	}
	
	void SpawnDIBunkerBasementWalls( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, float hoffset = 0.3 )
	{
		FP_SpawnObjectOnEdge(	origin,	6,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );

		FP_SpawnObjectOnEdge(	origin,	6,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	6,	2,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	3,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	4,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	6,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	7,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	8,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	25,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	8,	2,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	5,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	6,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	7,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	25,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		
			// SPAWN RIGHT BASEMENT HALLWAY WALLS
		FP_SpawnObjectOnEdge(	origin,	20,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );

		FP_SpawnObjectOnEdge(	origin,	20,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	20,	2,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	5,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	6,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	7,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	25,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	22,	2,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	3,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	4,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	6,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	7,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	8,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	25,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );	
	}
	
	void SpawnDIBunkerBasementEntranceWalls( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, float hoffset = 0.3 )
	{
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		//
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
	}

	void SpawnDIBunkerBasementEntranceL3Walls( vector origin, float hoffset = 0.3 )
	{
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	25,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	25,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
	}
	
	void SpawnDIBunkerBasementVentWalls( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, float hoffset = 0.3 )
	{
				// Vent Ents
		FP_SpawnObjectOnEdge(	origin,	8,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	8,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	8,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		
				// R2
		FP_SpawnObjectOnEdge(	origin,	13,	2,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	2,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	2,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R3
		FP_SpawnObjectOnEdge(	origin,	9,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	9,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	10,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	11,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	13,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	15,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	19,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	19,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R4
		FP_SpawnObjectOnEdge(	origin,	11,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	15,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R5
		FP_SpawnObjectOnEdge(	origin,	11,	5,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	5,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	5,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	5,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R6
		FP_SpawnObjectOnEdge(	origin,	11,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R7
		FP_SpawnObjectOnEdge(	origin,	11,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R8
		FP_SpawnObjectOnEdge(	origin,	12,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R9
		FP_SpawnObjectOnEdge(	origin,	9,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	9,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	10,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R10
		FP_SpawnObjectOnEdge(	origin,	10,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R11
		FP_SpawnObjectOnEdge(	origin,	10,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R12
		FP_SpawnObjectOnEdge(	origin,	10,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	13,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R13
		FP_SpawnObjectOnEdge(	origin,	10,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R14
		FP_SpawnObjectOnEdge(	origin,	10,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	11,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R15
		FP_SpawnObjectOnEdge(	origin,	12,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R16
		FP_SpawnObjectOnEdge(	origin,	12,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R17
		FP_SpawnObjectOnEdge(	origin,	10,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	11,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	15,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R18
		FP_SpawnObjectOnEdge(	origin,	10,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R19
		FP_SpawnObjectOnEdge(	origin,	10,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	13,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	15,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R20
		FP_SpawnObjectOnEdge(	origin,	10,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R21
		FP_SpawnObjectOnEdge(	origin,	10,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R22
		FP_SpawnObjectOnEdge(	origin,	10,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	11,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R23
		FP_SpawnObjectOnEdge(	origin,	12,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R24
		FP_SpawnObjectOnEdge(	origin,	9,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	9,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	10,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	11,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	19,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	19,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R25
		FP_SpawnObjectOnEdge(	origin,	14,	25,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	25,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R26
		FP_SpawnObjectOnEdge(	origin,	11,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	13,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	15,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
			// SPAWN BASEMENT ROOM ENTRANCE HALLWAYS
				
				// L1
		FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall",			1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	6,	5,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
				
				// L2
		FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_halfwall",			1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	6,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
				
				// L3
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall",			1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	6,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
				
				// L4
		FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_halfwall",			3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	22,	5,	"fmdpk_modscifiwalls_halfwall",			1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	22,	5,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
				
				// L5
		FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_halfwall",			3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	22,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
				
				// L6
		FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_halfwall",			3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	22,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
	}
	
	void SpawnDIBunkerEntranceF1Walls( vector origin, float voffset = FIRST_FLOOR_LOWER_HEIGHT_OFFSET, float hoffset = 0.3 )
	{
		FP_SpawnObjectOnEdge(	origin,	7,	25,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	25,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
	}
	
	void SpawnDIBunkerBasementCeilings( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 )
	{
		FP_SpawnObjectAtGrid(	origin,	6,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
			// SPAWN RIGHT HALLWAY CEILINGS
		FP_SpawnObjectAtGrid(	origin,	20,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
			// SPAWN SIDE ROOM HALLWAY CEILINGS
		FP_SpawnObjectAtGrid(	origin,	5,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	5,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	5,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	23,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	23,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	23,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
			// SPAWN LEFT SIDEROOM CEILINGS
		FP_SpawnObjectAtGrid(	origin,	2,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	2,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	2,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
			// SPAWN RIGHT SIDEROOM CEILINGS
		FP_SpawnObjectAtGrid(	origin,	24,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	24,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin, 26,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	24,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
	}
	
	void SpawnDIBunkerBasementVentCeilings( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 )
	{
		FP_SpawnObjectAtGrid(	origin,	13,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	9,	3,	"fmdpk_modscifiwalls_floor_slope",		180.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	10,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	11,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	13,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	15,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	19,	3,	"fmdpk_modscifiwalls_floor_slope",		0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	11,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	15,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	11,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	11,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	11,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	12,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	9,	9,	"fmdpk_modscifiwalls_floor_slope",		180.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	10,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	19,	9,	"fmdpk_modscifiwalls_floor_slope",		0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	13,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	11,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	12,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	12,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	11,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	15,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	13,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	15,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	11,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	12,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	9,	24,	"fmdpk_modscifiwalls_floor_slope",		180.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	10,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	11,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	19,	24,	"fmdpk_modscifiwalls_floor_slope",		0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	14,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	11,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	13,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	15,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
	}
}
class Layout_OpenAreaBunker : BaseLayout
{
	override void Spawn(vector origin)
	{
		const int W = 28;
		const int H = 28;

		// 1) BUILD GRID
		array<ref array<int>> grid;
		FP_GridInit(grid, W, H);

		// 2) DEFINE HALLS
		FP_GridFillRect(grid,  8,	1,	2,	7,	FP_HALL);
		FP_GridFillRect(grid,  8,	8,	10,	11,	FP_HALL);
		FP_GridFillRect(grid,  12,	19, 2,	7,	FP_HALL);
		FP_GridFillRect(grid,  13,	7,	1,	1,	FP_HALL);
		FP_GridFillRect(grid,  17,	7,	1,	1,	FP_HALL);
		FP_GridFillRect(grid,  11,	22,	1,	1,	FP_HALL);
		FP_GridFillRect(grid,  14,	22,	1,	1,	FP_HALL);
		FP_GridFillRect(grid,  5,	26,	16,	2,	FP_HALL);

		// 3) DEFINE ROOMS
		FP_GridFillRect(grid,	11,	2,	5,	5,	FP_ROOM);
		FP_GridFillRect(grid,	17,	2,	5,	5,	FP_ROOM);
		FP_GridFillRect(grid,	6,	20,	5,	5,	FP_ROOM);
		FP_GridFillRect(grid,	15,	20,	5,	5,	FP_ROOM);

		// 4) SPAWN FLOORS
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

		// 5) SPAWN DOORS
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

		// 6) SPAWN WARP ENTRANCE
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

		// 7) SPAWN WALLS
		FP_GenerateWalls(origin, grid, W, H, doorEdges);
		
		SpawnDIBunkerBasementWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		SpawnDIBunkerBasementWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		SpawnDIBunkerBasementHallwayWalls( origin );
		SpawnDIBunkerBasementRoom1( origin );
		SpawnDIBunkerBasementRoom2( origin );
		SpawnDIBunkerBasementRoom3( origin );
		SpawnDIBunkerBasementRoom4( origin );
		
		// 8) SPAWN CEILINGS
		for (int cy = 0; cy < H; cy++)
		{
			for (int cx = 0; cx < W; cx++)
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}
		
		SpawnDIBunkerBasementCeilings( origin );
		
		// 9) AUTO SPAWN LOOT IN 5x5, OR LARGER, ROOMS
		FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
		
		// 10) CUT HOLES IN FLOORS
		FP_DeleteFloorTile(	8,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	10,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	12,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	11,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	12,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	13,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	13,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	14,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	15,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	12,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	16,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	12,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	17,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	18,	W,	H,	grid,	floorObjs );
		
		// 11) SPAWN BASEMENT FLOORS
		SpawnDIBunkerBasementFloors( origin );
		SpawnOpenAreaRamps( origin );
		
		// 12) SPAWN FIRST FLOOR GREEBLES
		SpawnDIBunkerFirstFloorGreebles( origin );
		SpawnDIBunkerBasementGreebles( origin );
		
		// 13) SPAWN FIRST FLOOR TRAPS
		SpawnDIBunkerFloorTraps( origin );
		
		// 14 ) SPAWN TURRET TRAPS
		SpawnDIBunkerTurretTraps( origin );
	}
	
	void SpawnDIBunkerBasementFloors( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	8,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );

		FP_SpawnObjectAtGrid(	origin,	8,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );

		FP_SpawnObjectAtGrid(	origin,	8,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );

		FP_SpawnObjectAtGrid(	origin,	8,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	7,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	7,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	7,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	18,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	18,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	4,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	4,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	19,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	19,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
	}
	
	void SpawnDIBunkerBasementCeilings( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	7,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	7,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	7,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	18,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	18,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	4,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		
		FP_SpawnObjectAtGrid(	origin,	4,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		
		FP_SpawnObjectAtGrid(	origin,	19,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		
		FP_SpawnObjectAtGrid(	origin,	19,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
	}
	
	void SpawnDIBunkerFirstFloorGreebles( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	8,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	9,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	10,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	11,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	14,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	15,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		
		FP_SpawnObjectOnEdge(	origin,	12,	10,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	11,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	12,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	13,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	14,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	15,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	16,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	17,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	18,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		
		FP_SpawnObjectOnEdge(	origin,	13,	10,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	11,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	12,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	13,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	14,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	15,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	16,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	17,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	18,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		
		FP_SpawnObjectOnEdge(	origin,	16,	16,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	FP_GLASSRAIL_INSET );
		
	}
	
	void SpawnOpenAreaRamps( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	16,	15,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	15,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	14,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	14,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	13,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		FP_SpawnObjectAtGrid(	origin,	17,	13,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		
		FP_SpawnObjectAtGrid(	origin,	16,	12,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	17,	12,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	16,	11,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	11,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	10,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		FP_SpawnObjectAtGrid(	origin,	17,	10,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	10,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2,	FP_GLASSRAIL_INSET_2 );
		FP_SpawnObjectOnEdge(	origin,	16,	11,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1,	FP_GLASSRAIL_INSET_2 );
		FP_SpawnObjectOnEdge(	origin,	16,	12,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,		FP_GLASSRAIL_INSET_2 );
		FP_SpawnObjectOnEdge(	origin,	16,	13,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2,	FP_GLASSRAIL_INSET_2 );
		FP_SpawnObjectOnEdge(	origin,	16,	14,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1,	FP_GLASSRAIL_INSET_2 );
		FP_SpawnObjectOnEdge(	origin,	16,	15,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,		FP_GLASSRAIL_INSET_2 );
	}
	
	void SpawnDIBunkerBasementWalls( vector origin, float voffset )
	{
		FP_SpawnObjectOnEdge(	origin,	8,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	9,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	10,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	11,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	12,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	13,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	14,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	15,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	16,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	8,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	8,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	9,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	10,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	11,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	12,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	13,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	14,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	15,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	16,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	8,	8,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
	}
	
	void SpawnDIBunkerBasementHallwayWalls( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	8,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	7,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	10,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	10,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	7,	14,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	14,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	14,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	14,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );		
	}
	
	void SpawnDIBunkerFloorTraps( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	12,	11,	"fmdpk_modscifiwalls_floor_trap_2",		90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	13,	13,	"fmdpk_modscifiwalls_floor_trap_2",		90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	12,	16,	"fmdpk_modscifiwalls_floor_trap_2",		90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	12,	17,	"fmdpk_modscifiwalls_floor_trap_2",		90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
	}
	
	void SpawnDIBunkerTurretTraps( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	8,	2,	"fmdpk_modscifiwalls_turret_1",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	17,	8,	"fmdpk_modscifiwalls_turret_1",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	14,	"fmdpk_modscifiwalls_turret_1",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
	}
	
	void SpawnDIBunkerBasementRoom1( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	4,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	6,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	10,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	14,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );

		FP_SpawnObjectOnEdge(	origin,	4,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	4,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	4,	14,	"fmdpk_modscifiwalls_glasswall",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	14,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	14,	"fmdpk_modscifiwalls_glasswall",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	14,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	14,	"fmdpk_modscifiwalls_glasswall",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	14,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
	}
	
	void SpawnDIBunkerBasementRoom2( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	4,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	6,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	18,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );

		FP_SpawnObjectOnEdge(	origin,	4,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	4,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
	}
	
	void SpawnDIBunkerBasementRoom3( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	19,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );

		FP_SpawnObjectOnEdge(	origin,	19,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	19,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	18,	"fmdpk_modscifiwalls_doorframe_coded",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
	}
	
	void SpawnDIBunkerBasementRoom4( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	21,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );

		FP_SpawnObjectOnEdge(	origin,	19,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	10,	"fmdpk_modscifiwalls_doorframe_coded",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
	}
	
	void SpawnDIBunkerBasementGreebles( vector origin )
	{
		FP_SpawnLabDeskAgainstWall(			origin,	10,	18,	2,	0.65,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	18,	2,	0.65,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	18,	2,	0.65,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	18,	2,	0.65,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	16,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	16,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	16,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	16,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	15,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	15,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	15,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	15,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	14,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	14,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	14,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	14,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	13,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	13,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	13,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	13,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	12,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	12,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	12,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	12,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	11,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	11,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	11,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	11,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	11,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	17,	11,	1,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	12,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	17,	12,	1,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	13,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	17,	13,	1,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	15,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	16,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	17,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnStorageShelfAgainstWall(	origin,	19,	9,	0,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	19,	15,	2,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	20,	9,	0,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	20,	15,	2,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	21,	9,	0,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	21,	15,	2,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
	}
}
class Layout_11x11_3xFullGearRooms : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 11;
		const int H = 11;

	// 1) BUILD GRID
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

	// 2) DEFINE HALLWAYS
		FP_GridFillRect( grid,	0,	0,	11,	11,	FP_HALL );

	// 3) DEFINE ROOMS
		FP_GridFillRect( grid,	0,	6,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	6,	0,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	6,	6,	5,	5,	FP_ROOM );

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
			FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
	}
}
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

static ref BunkerAIZone g_FP_BunkerZone;

void SpawnObject(string objectName, vector position, vector orientation) {
        Object obj;
        obj = Object.Cast(GetGame().CreateObject(objectName, "0 0 0"));
        obj.SetPosition(position);
        obj.SetOrientation(orientation);
        // Force update collisions
        if (obj.CanAffectPathgraph()) {
                obj.SetAffectPathgraph(true, false);
                GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
        }
}

void main() {
        //INIT WEATHER BEFORE ECONOMY INIT------------------------
        //Weather weather = g_Game.GetWeather();
        //weather.MissionWeather(false);    // false = use weather controller from Weather.c
        //weather.GetOvercast().Set( Math.RandomFloatInclusive(0.4, 0.6), 1, 0);
        //weather.GetRain().Set( 0, 0, 1);
        //weather.GetFog().Set( Math.RandomFloatInclusive(0.05, 0.1), 1, 0);
        // SPAWN CUSTOM BUILDINGS FROM SKAERHEIM.TXT
      SpawnObject("Land_Mil_Barracks1", "19665.800781 73.404900 950.031982", "90.071480 0.000000 -0.000000");
      SpawnObject("Land_Mil_Barracks1", "19681.900391 73.496399 950.336975", "89.977852 0.000000 -0.000000");
      SpawnObject("Land_Mil_Barracks1", "19650.500000 73.388000 950.065002", "90.258797 0.000000 -0.000000");
      SpawnObject("Land_Mil_Barracks2", "19635.599609 72.764702 949.854980", "89.977905 0.000000 -0.000000");
      SpawnObject("Land_Mil_Barracks3", "19615.300781 73.328499 951.588989", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_Mil_Barracks4", "19595.900391 72.862999 951.267029", "90.353210 0.000000 -0.000000");
      SpawnObject("Land_Mil_Barracks5", "19566.699219 76.987000 943.593018", "89.321312 0.000000 -0.000000");
      SpawnObject("Land_Power_Station", "19698.699219 73.270798 949.327026", "89.414803 0.000000 -0.000000");
      SpawnObject("Land_Tower_TC3_Red", "19531.000000 71.333000 948.708008", "-4.222116 0.000000 -0.000000");
      SpawnObject("land_tradepost_heli", "19576.599609 71.242302 864.804993", "0.000000 0.000000 -0.000000");
      SpawnObject( "Land_Garage_Row_Big", "19524.400391 74.744904 916.880981", "-90.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Tank_Big", "19500.500000 76.656502 894.083008", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Tank_Big", "19498.900391 76.332199 872.257996", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Power_TransformerStation_Big", "19523.800781 73.935402 894.715027", "-0.003497 0.000000 0.000000" );
      SpawnObject( "Land_Mil_ATC_Big", "19535.300781 87.027100 759.929993", "58.723881 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Fortified_Nest_Big", "19542.400391 72.212196 725.585022", "58.723881 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Fortified_Nest_Small", "19544.400391 72.077904 736.166992", "58.723881 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Fortified_Nest_Watchtower", "19600.099609 73.361801 656.302979", "-106.305595 0.000000 -0.000000" );
      SpawnObject( "land_tradepost_heli", "19627.599609 71.181099 688.992981", "-148.338333 0.000000 -0.000000" );
      SpawnObject( "Land_Medical_Tent_Big", "19676.500000 71.299698 766.078003", "37.716530 0.000000 -0.000000" );
      SpawnObject( "Land_Medical_Tent_Big", "19682.099609 71.328796 779.919983", "3.190965 0.000000 -0.000000" );
      SpawnObject( "Land_Medical_Tent_Big", "19677.599609 71.269302 794.560974", "-34.810341 0.000000 0.000000" );
      SpawnObject( "Land_Mil_Tent_Big2_2", "19644.500000 73.286698 750.835022", "28.991518 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big2_3", "19632.400391 73.275101 761.309021", "69.149002 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big2_4", "19635.099609 73.261302 796.357971", "111.279083 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big2_5", "19646.400391 73.282099 808.554993", "-30.307367 0.000000 0.000000" );
      SpawnObject( "Land_Mil_Tent_Big3", "19646.800781 72.349001 780.171997", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big4", "19623.599609 72.496498 778.098022", "87.240410 0.000000 -0.000000" );
      SpawnObject( "Land_Geoplant_CoolingStack", "19700.300781 71.256798 671.067993", "179.671265 0.000000 -0.000000" );
      SpawnObject( "Land_Geoplant_Well_Round", "19705.800781 70.996101 687.101990", "105.540787 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Radome_Small", "19518.699219 69.283897 719.716980", "17.174812 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Antena_K7", "19516.900391 90.056602 762.184021", "58.723881 0.000000 -0.000000" );
      SpawnObject( "Land_MobileLaboratory", "19668.000000 71.681900 761.984009", "148.428665 0.000000 -0.000000" );
      SpawnObject( "di_abandoned_warehouse", "19715.300781 70.982903 839.489014", "0.656780 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_03", "19719.599609 70.933899 799.849976", "89.366951 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_04", "19697.199219 68.802200 775.934998", "8.257126 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_03", "19718.699219 70.537697 757.487976", "-81.911240 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_03", "19728.500000 69.600403 793.039001", "74.775604 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_FireStation", "19714.099609 71.496696 715.885010", "90.895279 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_apart1", "19733.599609 76.984299 768.599976", "-85.478058 0.000000 0.000000" );
      SpawnObject( "ANT_rock_apart2", "19741.800781 73.621902 769.541992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_bright_apart1", "19745.400391 74.346001 767.510986", "-56.484890 0.000000 0.000000" );
      SpawnObject( "ANT_rock_bright_apart2", "19744.500000 75.328400 769.536011", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_bright_monolith2", "19749.400391 71.817101 773.013000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_bright_wallh2", "19662.500000 63.745399 650.424011", "101.898155 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_bright_wallh3", "19627.400391 66.295197 643.398010", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_bright_spike3", "19623.599609 69.798401 643.393005", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_monolith1", "19617.199219 64.775299 651.375977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_monolith3", "19607.599609 70.314796 646.820984", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_bright_apart1", "19603.300781 72.567398 644.161011", "37.716530 0.000000 -0.000000" );
      SpawnObject( "Land_FlatRock_1", "19571.099609 70.764198 644.726990", "-87.717422 0.000000 0.000000" );
      SpawnObject( "Land_FlatRock_2", "19596.900391 70.515503 645.583984", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FlatRock_4", "19565.500000 70.427299 648.568970", "74.775604 0.000000 -0.000000" );
      SpawnObject( "Land_FlatRock_1", "19588.099609 70.641197 646.004028", "92.193031 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "19583.900391 69.725197 640.301025", "124.130798 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_Rocks1", "19587.400391 71.495796 642.275024", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small", "19695.400391 67.516502 854.773010", "0.563035 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small", "19720.800781 67.526901 879.757019", "0.468611 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19720.900391 69.379204 917.338989", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19714.599609 69.422401 954.973999", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19677.199219 69.367203 954.995972", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19639.699219 69.353104 955.010010", "-179.954529 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19602.900391 69.393600 955.036987", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19565.300781 69.424301 955.033997", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19565.300781 69.394203 929.909973", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19533.800781 69.397697 955.054016", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Barracks6_Old", "19603.599609 71.663696 922.466980", "179.111694 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Barracks6_Old", "19645.400391 71.697098 922.956970", "178.737442 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Barracks6_Old", "19686.199219 71.816498 923.877991", "178.922836 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Airfield_HQ", "19688.800781 76.843803 897.273010", "-1.031687 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_Artilery_Rampart", "19740.300781 72.415298 872.341980", "-89.366951 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_Artilery_Rampart", "19740.099609 72.375298 881.596008", "-91.266090 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_Artilery_Rampart", "19739.500000 72.188400 891.575012", "-98.138313 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_Artilery_Rampart", "19739.000000 72.087898 900.836975", "-90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_Artilery_Rampart", "19739.000000 72.121696 910.421021", "-90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_Artilery_Rampart", "19740.099609 71.834900 919.216980", "-75.624390 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_Artilery_Rampart", "19742.199219 71.836098 927.432983", "-77.505913 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19681.000000 71.293701 974.484985", "3.565802 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19672.599609 71.277702 974.413025", "-0.937253 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19663.900391 71.163498 974.052002", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19655.300781 71.114799 974.101990", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19647.099609 71.120003 973.755981", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19638.599609 71.294899 973.443970", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19630.199219 71.256897 973.275024", "0.937253 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19621.599609 71.174103 973.325989", "0.842821 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_Artilery_Nest", "19606.500000 72.469101 978.778992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_04", "19572.500000 71.055603 988.002991", "-102.083305 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19589.500000 71.578102 974.073975", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19581.500000 71.693703 973.197998", "-10.038879 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19572.699219 71.510201 972.213989", "-3.096402 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19564.099609 71.353996 972.689026", "10.038879 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19555.500000 71.396400 973.866028", "6.286324 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19546.900391 71.500504 973.984985", "-0.751889 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19538.199219 71.642097 973.653992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19529.900391 71.856697 973.489014", "1.031687 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19521.099609 72.202599 973.241028", "0.094421 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19515.400391 72.183800 969.645020", "-84.552368 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19515.000000 72.281799 961.208008", "-87.467651 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19514.800781 72.430702 954.781982", "93.525139 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19514.500000 72.430702 940.361023", "92.001938 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_Big", "19514.699219 72.430702 948.190002", "93.289780 0.000000 -0.000000" );
      SpawnObject( "flip_workshop", "19597.099609 75.449600 865.710022", "-0.563035 0.000000 0.000000" );
      SpawnObject( "flip_workshop", "19597.500000 75.449600 846.974976", "-0.657461 0.000000 0.000000" );
      SpawnObject( "flip_workshop", "19597.699219 75.449600 828.581970", "-0.751889 0.000000 0.000000" );
      SpawnObject( "Land_Airport_Small_Hangar2", "19530.099609 74.681198 839.994019", "-88.903564 0.000000 0.000000" );
      SpawnObject( "StaticObj_Stockyard_OreMound1", "19511.400391 71.407303 872.112976", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_ind_house_1", "19600.599609 73.212303 886.223999", "-0.374188 0.000000 0.000000" );
      SpawnObject( "Land_Mil_Barracks_Round", "19579.800781 72.514900 906.530029", "-179.667755 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Barracks_Round", "19570.199219 72.550903 906.476013", "-179.863617 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Barracks_Round", "19560.900391 72.539803 906.517029", "-179.951035 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Barracks_Round", "19550.900391 72.517899 906.518982", "-179.856613 0.000000 -0.000000" );
      SpawnObject( "Land_Cliff_1", "19733.800781 62.744801 683.411011", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cliff_1", "19735.800781 65.886002 698.112976", "0.000000 -85.995468 -0.000000" );
      SpawnObject( "Land_Cliff_1", "19740.099609 66.405296 715.023010", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cliff_1", "19745.199219 68.547203 726.635010", "0.000000 0.000000 -80.999916" );
      SpawnObject( "Land_Cliff_1", "19741.800781 62.144600 735.163025", "132.853653 0.000000 -0.000000" );
      SpawnObject( "Land_Cliff_1", "19738.500000 61.025902 742.489014", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_ControlTower", "19574.599609 79.029198 651.999023", "112.589767 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Fortified_Nest_Watchtower", "19558.699219 73.015297 676.026001", "-13.134274 0.000000 0.000000" );
      SpawnObject( "ANT_mil_artilery_rampart", "19511.400391 72.245697 767.109985", "72.526787 0.000000 -0.000000" );
      SpawnObject( "ANT_mil_artilery_rampart", "19509.199219 72.217598 776.848022", "81.335121 0.000000 -0.000000" );
      SpawnObject( "ANT_mil_artilery_rampart", "19508.500000 71.918999 786.763977", "90.633041 0.000000 -0.000000" );
      SpawnObject( "ANT_mil_artilery_rampart", "19508.699219 71.809998 796.557007", "92.193031 0.000000 -0.000000" );
      SpawnObject( "ANT_mil_artilery_rampart", "19508.800781 71.760201 806.116028", "90.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_mil_artilery_rampart", "19509.099609 71.689697 815.398010", "93.289780 0.000000 -0.000000" );
      SpawnObject( "ANT_mil_artilery_rampart", "19509.500000 71.776901 825.070007", "92.759575 0.000000 -0.000000" );
      SpawnObject( "ANT_mil_artilery_rampart", "19510.000000 71.449303 834.312012", "91.899193 0.000000 -0.000000" );
      SpawnObject( "ANT_mil_artilery_rampart", "19509.800781 71.880699 843.635010", "86.590508 0.000000 -0.000000" );
      SpawnObject( "ANT_mil_artilery_rampart", "19506.500000 71.838402 849.914978", "38.937992 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_13", "19511.300781 71.794899 928.299988", "-3.846153 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_13", "19511.000000 72.884697 914.340027", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_13", "19512.500000 71.135498 904.551025", "168.219498 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_13", "19513.400391 70.728104 891.908020", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big2_5", "19663.300781 73.369903 750.422974", "-14.636485 0.000000 0.000000" );
      SpawnObject( "Land_Mil_Tent_Big3", "19659.900391 72.367498 779.622986", "179.958023 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big4", "19651.500000 72.490104 762.471985", "-65.679344 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19551.199219 71.235497 699.664001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19551.099609 71.239197 680.487000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19562.599609 71.226097 669.406006", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19568.800781 71.221802 687.810974", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19570.900391 71.329803 699.862976", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19571.000000 71.375702 688.013977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19571.000000 71.359901 669.427979", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19568.800781 71.151299 709.890015", "-39.502235 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19550.199219 71.181602 713.739014", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_apart1", "19545.000000 70.365402 709.716003", "32.275883 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_apart1", "19543.000000 71.615898 708.809021", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_apart1", "19542.099609 73.186096 702.741028", "73.276115 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_apart1", "19514.699219 71.516296 709.599976", "-151.240097 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_apart1", "19518.500000 71.754997 712.541992", "-43.722168 0.000000 0.000000" );
      SpawnObject( "ANT_rock_apart1", "19524.099609 72.690002 711.443970", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_apart1", "19510.000000 66.604897 717.270020", "180.000000 -89.965729 -0.000000" );
      SpawnObject( "ANT_rock_apart1", "19506.900391 71.706398 719.577026", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_apart1", "19508.900391 70.867798 724.098022", "60.049271 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_apart1", "19508.599609 70.886398 713.914001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19578.900391 71.089104 670.038025", "-16.699579 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19584.500000 71.187698 702.523010", "35.839043 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19589.699219 71.114601 690.719971", "-67.174690 0.000000 0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19551.599609 72.076302 709.466980", "-4.126596 0.000000 0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19559.000000 72.091904 709.663025", "2.158194 0.000000 -0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19566.199219 72.168503 709.549988", "-0.374188 0.000000 0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19577.500000 72.186302 709.658997", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19573.800781 72.243301 709.658020", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19580.699219 72.076302 705.380005", "88.584480 0.000000 -0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19581.000000 72.192398 697.919983", "-90.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19581.000000 72.149399 690.460999", "90.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19581.199219 72.232201 683.025024", "89.104713 0.000000 -0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19581.300781 72.196198 675.580994", "89.366951 0.000000 -0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19581.300781 72.216400 668.281006", "-88.100800 0.000000 0.000000" );
      SpawnObject( "ANT_wall_cncbarrier_4block", "19581.000000 72.216400 662.715027", "-85.013901 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19657.900391 71.305702 773.530029", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19647.500000 71.253403 762.888000", "49.255650 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19661.699219 71.220398 754.317017", "-6.381325 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19642.800781 71.237297 753.237000", "-0.842821 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19623.300781 71.249496 753.197998", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19608.800781 71.276398 758.101013", "-16.138136 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19681.199219 71.271103 755.348999", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19694.500000 71.220596 758.846008", "-30.307367 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19696.199219 71.229103 806.315979", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19677.800781 71.235199 803.176025", "-9.662643 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19657.699219 71.244698 807.073975", "22.141302 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19641.599609 71.271500 810.879028", "-85.752510 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19633.199219 71.287903 807.505981", "41.657230 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19624.400391 71.290497 793.658997", "23.549181 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19617.000000 71.287201 778.424011", "28.524796 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19610.300781 71.252701 767.723022", "-56.295357 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_03", "19690.300781 67.453003 762.419006", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19682.900391 71.275497 774.835999", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19690.599609 71.266098 790.043030", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19668.400391 71.236801 787.968994", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19674.500000 71.263199 768.719971", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19668.000000 71.265602 800.010010", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19681.000000 71.220703 790.033020", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19654.599609 71.251701 791.898987", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19639.300781 71.265900 791.585999", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19641.000000 71.259003 773.301025", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19626.300781 71.237297 769.038025", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19576.900391 71.136902 762.861023", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19589.500000 71.110100 756.374023", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19594.800781 70.977898 779.791016", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19593.800781 70.991203 767.997986", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19570.199219 70.959801 783.252014", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19583.199219 70.946297 791.495972", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19595.800781 71.108299 739.435974", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19574.599609 71.131798 728.520020", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19570.699219 71.088097 748.056030", "79.092384 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19705.400391 71.218803 806.820984", "0.374188 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19610.000000 71.235397 754.017029", "73.557938 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19614.699219 71.262604 749.870972", "8.914954 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19631.400391 71.250801 749.708008", "-7.318239 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19649.500000 71.232597 752.716980", "-11.166721 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19668.400391 71.225098 805.765015", "4.035442 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Large", "19630.000000 71.242302 779.531982", "2.063710 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19692.199219 71.799004 814.356018", "-97.575508 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19686.699219 71.799004 811.846008", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19681.400391 71.799004 814.583008", "115.123390 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19674.300781 71.799004 814.679993", "-101.235397 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19669.500000 71.799004 812.104004", "173.576431 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19664.500000 71.330101 812.471985", "50.006134 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19662.199219 71.799004 814.357971", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19655.699219 71.788002 818.344971", "-59.763325 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19650.500000 71.814499 818.377991", "-105.927666 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19642.400391 71.814796 818.690979", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19636.199219 71.646400 819.674011", "-93.637115 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19628.699219 71.653297 814.669983", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19625.400391 71.290604 811.796021", "59.014744 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19618.699219 71.672897 804.481018", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19615.500000 71.723297 796.773987", "11.444889 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19612.099609 71.529404 788.926025", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19601.900391 71.655197 772.090027", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19607.300781 71.749901 779.156982", "68.865921 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19611.000000 71.718201 784.528015", "-152.185287 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19598.400391 71.742302 763.515991", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19599.800781 71.733200 756.093994", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19600.699219 71.759499 747.435974", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19604.400391 71.805603 745.539978", "74.303337 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19603.000000 71.591904 755.117981", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19603.199219 71.115402 764.843994", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19609.800781 71.700302 743.919006", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19617.500000 71.805901 745.400024", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19613.900391 71.805901 746.278992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19611.000000 71.732903 741.260986", "-75.723320 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19616.800781 71.759499 739.051025", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19624.599609 71.686401 738.676025", "-94.103409 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19631.800781 71.245903 739.973999", "-86.201088 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19626.699219 71.794098 749.950012", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19618.199219 71.819702 754.499023", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19620.599609 71.438400 739.460999", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19639.699219 71.623398 742.974976", "-98.236534 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19620.400391 71.805901 749.432007", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_14", "19615.699219 71.504601 752.741028", "-110.712997 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_13", "19608.199219 71.782700 758.179993", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_13", "19691.300781 71.636902 747.718018", "99.073143 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2fb", "19626.900391 80.267502 749.395996", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2fb", "19621.500000 79.598198 744.958008", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2fb", "19617.599609 80.019096 742.382019", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2s", "19621.300781 78.857002 748.731018", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2s", "19609.500000 79.298500 754.499023", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1s", "19611.300781 72.319801 747.140991", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1s", "19609.699219 72.756798 752.963013", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1s", "19624.300781 71.745598 747.844971", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1s", "19627.300781 71.733803 741.698975", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1s", "19629.500000 71.733803 752.934998", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1s", "19617.900391 71.770203 767.260010", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1s", "19624.199219 71.733803 758.760010", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1s", "19616.300781 71.759399 760.247986", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2f", "19620.300781 80.892998 742.565979", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2f", "19628.300781 80.824303 746.356995", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2f", "19627.300781 80.490303 743.695984", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2f", "19634.099609 80.490303 747.578003", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2f", "19632.500000 80.490303 742.465027", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19636.800781 72.515602 746.799011", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19630.699219 72.515602 746.468994", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19615.699219 73.029999 744.921021", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19616.699219 73.424599 739.867004", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19623.900391 73.330803 738.419006", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19613.400391 72.552002 772.168030", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19612.300781 72.541199 767.213013", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19610.800781 72.552002 770.937988", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19607.000000 72.552002 773.862000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19611.699219 72.552002 776.385986", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19609.300781 72.552002 775.653992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19606.099609 73.828499 777.190002", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19611.099609 73.528198 780.362976", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_carpinus_2s", "19605.500000 73.206398 769.645996", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_carpinus_2s", "19659.800781 73.198402 814.106018", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_carpinus_2s", "19677.599609 73.188904 811.575012", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_carpinus_2s", "19734.400391 71.661598 810.354004", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_carpinus_2s", "19696.099609 80.338501 753.073975", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_carpinus_2s", "19683.400391 78.627098 750.302979", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2fb_summer", "19638.000000 79.394798 814.778992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2fb_summer", "19634.900391 79.394798 813.637024", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2fb_summer", "19631.300781 79.394798 811.184998", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2fb_summer", "19629.599609 79.394798 808.968018", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2sb_summer", "19625.699219 77.773804 807.661011", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2sb_summer", "19633.199219 77.773804 812.372986", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2sb_summer", "19635.900391 77.773804 807.807007", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2s_summer", "19629.400391 78.024696 805.125977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_3fb_summer", "19624.699219 80.639297 801.323975", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_3s", "19626.000000 80.002998 802.590027", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2sb_summer", "19621.900391 77.776299 798.580017", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f_summer", "19618.699219 72.555298 792.276978", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f_summer", "19623.500000 72.555298 802.690979", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f_summer", "19633.000000 72.766800 817.090027", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f_summer", "19628.400391 73.493202 813.458008", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f_summer", "19624.099609 73.708099 809.793030", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2fb_summer", "19694.599609 79.335999 807.028015", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2fb_summer", "19707.099609 79.528503 810.859985", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_2s_summer", "19701.000000 78.309502 805.374023", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19694.599609 72.530899 797.252014", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19701.099609 72.493896 810.921997", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1f", "19692.000000 72.493896 808.682007", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Wreck_UH1Y", "19655.099609 99.777298 704.299988", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Office2", "19665.699219 79.643799 730.874023", "-7.036265 0.000000 0.000000" );
      SpawnObject( "Land_Airport_Small_Hangar2", "19529.400391 74.671600 798.325989", "-89.366951 0.000000 0.000000" );
      SpawnObject( "Land_Cliff_1", "19724.900391 68.318703 768.479004", "0.000000 -87.030403 -0.000000" );
      SpawnObject( "Land_Cliff_1", "19730.699219 66.067398 785.674988", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cliff_1", "19719.599609 68.913696 795.984985", "97.575508 0.000000 -0.000000" );
      SpawnObject( "Land_Cliff_1", "19705.400391 66.698196 774.484009", "0.000000 -23.000986 -30.998430" );
      SpawnObject( "Land_Mil_Barracks5", "19642.400391 77.053398 895.984985", "-0.563035 0.000000 0.000000" );
      SpawnObject( "Land_Mil_GuardTower", "19726.500000 77.581001 890.890015", "-91.899193 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_ReinforcedTank1", "19646.400391 71.305199 825.091003", "153.404236 0.000000 -0.000000" );
      SpawnObject( "Land_HA_Heli_1", "19605.500000 100.074997 863.890015", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19564.800781 71.106300 804.914001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19564.400391 71.206100 843.140015", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19578.000000 71.132202 819.507996", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19634.699219 71.017799 851.122986", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19654.500000 71.109398 853.784973", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19645.000000 71.211601 862.854004", "-25.238916 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19616.900391 71.152298 821.806030", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19619.300781 71.069603 838.911011", "-59.203484 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19667.599609 71.166199 899.794006", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19670.800781 71.108299 882.286011", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19619.099609 71.032997 910.421021", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19642.400391 71.101601 910.991028", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19661.500000 71.092903 917.856995", "38.092930 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19596.099609 71.080299 935.523010", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19622.599609 71.119301 932.679016", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19668.599609 71.085403 962.893982", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19645.800781 71.213203 963.435974", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19661.199219 71.132797 939.405029", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19625.900391 71.161598 948.637024", "39.031178 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19583.099609 71.149597 938.310974", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19558.500000 71.092903 913.127991", "45.129791 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19578.199219 71.194298 912.211975", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19583.699219 71.068001 889.668030", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19550.500000 71.131599 878.997986", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19559.800781 71.065903 863.002991", "-40.626202 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19538.800781 71.168900 897.700012", "-33.495514 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19540.900391 71.091400 935.747009", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19535.500000 71.125504 965.171021", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19582.800781 71.210403 953.414001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19672.800781 71.203499 831.778992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19668.300781 71.120003 846.898010", "29.179558 0.000000 -0.000000" );
      SpawnObject( "Land_HA_Cabin_12", "19683.800781 71.135902 866.268982", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_HA_Cabin_12", "19669.800781 71.204201 864.291992", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_GasMeterCabinet", "19669.900391 71.819801 856.786011", "-179.573334 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1s", "19553.000000 72.303497 697.330994", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_pinussylvestris_1s", "19550.500000 72.284103 699.148987", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_betulapendula_3fc", "19549.800781 79.595901 692.549988", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_d_betulapendula_fallen", "19555.400391 71.272499 697.966980", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_d_betulapendula_fallen", "19558.900391 71.272499 691.737000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_d_betulapendula_stump", "19557.199219 73.002502 695.630981", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_betulapendulae_2f_summer", "19552.900391 74.544601 700.723022", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_betulapendulae_2f_summer", "19556.000000 74.544601 704.802002", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_betulanana_1s_summer", "19551.900391 71.766197 704.340027", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_betulanana_1s_summer", "19552.400391 71.752197 706.271973", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_betulanana_1s_summer", "19550.099609 71.897202 705.653992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_betulapendulae_3f_summer", "19561.000000 73.946503 694.703003", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_betulapendulae_1s_summer", "19566.300781 72.038902 693.932983", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_betulapendula_2fb", "19563.599609 78.321198 705.525024", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_betulapendula_1fb", "19561.699219 72.733002 694.651001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_betulapendula_1fb", "19566.400391 72.687103 701.046997", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_betulapendulae_3s_summer", "19572.300781 73.409599 705.997986", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_fagussylvatica_1s", "19565.000000 74.362000 660.708984", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_fagussylvatica_2s", "19562.500000 79.070801 664.726990", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_fagussylvatica_1fc_summer", "19567.900391 76.451202 662.405029", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_6m", "19565.099609 71.797401 659.416016", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_6m", "19570.699219 71.733299 659.502991", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Mil_HBarrier_6m", "19575.199219 71.700897 659.539001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_small", "19579.099609 71.348396 659.648987", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_PM_Forklift", "19732.900391 72.875397 865.507996", "40.626324 0.000000 -0.000000" );
      SpawnObject( "ANT_misc_woodencrate_5x", "19734.800781 72.230797 871.044006", "-21.768023 0.000000 0.000000" );
      SpawnObject( "ANT_misc_woodencrate_5x", "19734.099609 72.230797 876.918030", "-160.251587 0.000000 -0.000000" );
      SpawnObject( "ANT_misc_woodencrate_5x", "19735.500000 72.230797 875.481018", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_misc_woodencrate_5x", "19735.599609 72.230797 878.247009", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_misc_woodencrate_5x", "19734.800781 72.230797 873.276001", "22.050720 0.000000 -0.000000" );
      SpawnObject( "ANT_misc_woodencrate_5x", "19735.500000 72.230797 876.810974", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_misc_woodencrate_5x", "19733.400391 72.230797 879.122986", "20.452225 0.000000 -0.000000" );
      SpawnObject( "ANT_misc_woodencrate_5x", "19735.199219 72.230797 880.802002", "24.111162 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "19733.400391 71.563499 875.052002", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "19732.500000 71.523903 877.528992", "-42.221386 0.000000 0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "19733.300781 71.507004 871.833984", "49.352188 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_3x", "19733.300781 71.924400 882.987000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19735.300781 71.597801 868.643982", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_PalletTrolley_01_yellow_F", "19732.500000 71.931702 869.431030", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets2", "19736.000000 71.598000 867.056030", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_3s_summer", "19733.800781 83.268402 754.125977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_piceaabies_1f_summer", "19733.099609 72.459297 748.838989", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_piceaabies_1f_summer", "19730.099609 72.848602 748.848999", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_piceaabies_1f_summer", "19731.099609 72.411400 748.580994", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_piceaabies_1f_summer", "19735.699219 72.470497 751.210999", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_piceaabies_1f_summer", "19729.900391 73.249397 751.039001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_piceaabies_1f_summer", "19728.099609 74.403198 754.164001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_piceaabies_1f_summer", "19728.900391 74.031898 753.291992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_piceaabies_1f_summer", "19729.300781 73.744400 752.541992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_piceaabies_1f_summer", "19731.900391 75.689598 762.120972", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_piceaabies_1f_summer", "19730.099609 75.571503 759.184021", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_piceaabies_1f_summer", "19728.699219 75.328102 757.379028", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_d_piceaabies_fallend", "19727.699219 73.329300 753.612976", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_1f_summer", "19726.199219 77.389801 754.789978", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_1f_summer", "19726.900391 76.024696 750.429016", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_1sb_summer", "19724.599609 74.839500 751.765991", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_1sb_summer", "19743.500000 74.669998 792.797974", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_1sb_summer", "19744.300781 75.744102 787.351990", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_1sb_summer", "19739.300781 75.908600 791.526001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_1sb_summer", "19730.000000 77.159698 795.674011", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_1s_summer", "19736.900391 75.017502 795.797974", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_1s_summer", "19739.000000 77.171600 787.018982", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_2fb_summer", "19737.099609 84.083099 790.999023", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_3f_summer", "19739.000000 86.423203 781.198975", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_3f_summer", "19719.300781 90.533798 785.539978", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_3s_summer", "19712.599609 88.855301 785.539001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_3s_summer", "19483.500000 82.182404 869.129028", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_3s_summer", "19487.900391 81.728699 852.408020", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_piceaabies_3s_summer", "19769.000000 82.292297 886.421997", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_bright_monolith3", "19570.699219 69.488503 689.228027", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_bright_monolith3", "19571.400391 67.627602 698.120972", "39.001213 -61.998390 -0.000280" );
      SpawnObject( "ANT_rock_wallh4", "19570.300781 68.566200 674.515015", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tower_Small", "19719.199219 75.065598 957.190979", "38.655312 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_GuardTower", "19597.000000 77.473801 973.463013", "-178.268677 0.000000 -0.000000" );
      SpawnObject( "land_cablereel", "19735.500000 71.730904 883.427002", "0.000000 0.000000 90.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19732.800781 71.704102 819.786011", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19732.800781 71.722198 821.142029", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19731.800781 71.766098 818.489014", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19732.199219 72.735100 819.268982", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19729.300781 71.753700 819.778992", "-20.923227 0.000000 0.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19728.500000 71.749802 820.703003", "-37.809429 0.000000 0.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19725.800781 71.756401 819.934021", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19732.300781 71.704102 823.593994", "-59.576344 0.000000 0.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19728.500000 72.709396 819.697021", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19727.900391 71.755798 819.380981", "18.390438 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19732.599609 72.668198 820.684021", "10.885257 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_BoxWooden", "19732.500000 71.704102 825.008972", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_WoodBox1", "19732.400391 72.437401 824.919006", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_WoodBox1", "19732.199219 72.432503 823.645996", "32.462200 0.000000 -0.000000" );
      SpawnObject( "Land_WoodBox1", "19732.400391 72.904900 824.221008", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "19727.500000 72.196999 822.594971", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "19727.099609 72.172600 825.085999", "-72.808510 0.000000 0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_3x", "19724.699219 71.901100 823.958984", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "19723.599609 71.483299 822.403992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "19723.800781 71.522903 820.159973", "145.427826 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "19730.599609 71.444099 821.645020", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "19729.699219 71.496101 824.827026", "87.548073 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19728.800781 71.845802 828.598022", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19718.900391 71.921402 822.533997", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19721.199219 71.953598 825.620972", "-46.910343 0.000000 0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets2", "19719.800781 71.587196 824.335022", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallet", "19719.900391 71.827202 827.065002", "0.000000 54.000374 0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19733.199219 71.973999 840.521973", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19733.000000 71.973999 837.781982", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19732.699219 71.578499 834.836975", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19732.900391 73.473503 837.799988", "4.410646 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19732.900391 74.999199 837.823975", "-5.818607 0.000000 0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19733.099609 73.485802 840.520996", "-4.035442 0.000000 0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19731.000000 71.973999 839.422974", "37.809429 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19731.000000 71.076698 837.354004", "19.796721 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_table_broken", "19732.400391 71.539902 831.698975", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_chair_broken", "19731.300781 71.213501 831.645996", "-63.144783 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_shelfs_small_crushed", "19732.400391 71.208199 829.104004", "28.429314 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_shelfs_small_crushed", "19732.699219 71.240700 827.755005", "-42.785858 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_shelfs_small_crushed", "19732.300781 71.284897 827.008972", "0.000000 6.001426 0.000000" );
      SpawnObject( "Sewer_Door_Small_Drain", "19733.099609 71.157700 836.145996", "91.096420 0.000000 -0.000000" );
      SpawnObject( "Sewer_Door_Small_Drain", "19731.099609 71.157700 836.192017", "-88.209427 0.000000 0.000000" );
      SpawnObject( "Sewer_Door_Small_Drain", "19729.199219 71.157700 836.250000", "91.550667 0.000000 -0.000000" );
      SpawnObject( "Sewer_Door_Small_Drain", "19727.300781 71.155502 836.299988", "91.415512 0.000000 -0.000000" );
      SpawnObject( "Sewer_Door_Small_Drain", "19725.400391 71.157700 836.344971", "-88.584480 0.000000 0.000000" );
      SpawnObject( "Sewer_Door_Small_Drain", "19723.500000 71.157700 836.382996", "-88.903564 0.000000 0.000000" );
      SpawnObject( "Sewer_Door_Small_Drain", "19721.599609 71.157700 836.421997", "-88.903564 0.000000 0.000000" );
      SpawnObject( "Sewer_Door_Small_Drain", "19719.699219 71.157700 836.460999", "91.096420 0.000000 -0.000000" );
      SpawnObject( "Sewer_Door_Small_Drain", "19717.800781 71.157700 836.495972", "91.266090 0.000000 -0.000000" );
      SpawnObject( "Sewer_Door_Small_Drain", "19715.900391 71.157700 836.536011", "91.266090 0.000000 -0.000000" );
      SpawnObject( "Land_white_bag", "19717.099609 71.554298 821.645996", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_white_bag", "19716.900391 71.603798 820.013977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_white_bag", "19716.300781 71.549500 821.500977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_white_bag", "19718.199219 71.567902 819.409973", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_white_bag", "19715.699219 71.600197 820.655029", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_wood_palet_1", "19716.500000 71.326401 824.294983", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_rubbish_floor_1", "19715.400391 71.249199 822.857971", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_rubbish_floor_1", "19715.500000 71.245697 819.041016", "-98.711166 0.000000 -0.000000" );
      SpawnObject( "Land_rubbish_floor_1", "19718.599609 71.234299 820.882019", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_plastic_container_2", "19715.000000 71.420998 821.848999", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_plastic_container", "19719.300781 71.430901 819.499023", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_fire_bin_1", "19713.900391 71.777397 819.568970", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_fire_bin_1", "19712.800781 71.671204 820.401978", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_broken_tiles", "19714.000000 71.235001 821.369995", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_box_3", "19716.000000 71.552803 823.945984", "28.052204 0.000000 -0.000000" );
      SpawnObject( "Land_box_3", "19716.300781 71.582397 824.619995", "20.923227 0.000000 -0.000000" );
      SpawnObject( "Land_box_4", "19714.599609 71.396202 823.994019", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_box_1", "19714.800781 71.413200 825.077026", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_box_1", "19717.000000 71.413200 825.598999", "-90.895279 0.000000 -0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19712.000000 71.812897 824.619995", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19711.599609 71.812897 824.114990", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19711.400391 71.812897 825.138000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19711.500000 71.812897 823.302979", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19711.099609 71.812897 823.530029", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19711.000000 71.812897 825.456970", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19712.599609 71.812897 825.804016", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19711.199219 71.812897 826.232971", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19709.000000 71.812897 826.745972", "-90.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19708.599609 71.812897 826.148987", "91.550667 0.000000 -0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19708.099609 71.812897 825.221008", "-11.634057 0.000000 0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19708.199219 71.812897 822.815002", "-11.259414 0.000000 0.000000" );
      SpawnObject( "Land_Rusty_iron_fence", "19708.099609 71.812897 823.346008", "-13.040925 0.000000 0.000000" );
      SpawnObject( "land_flip_workbench_yellow", "19709.400391 71.965401 818.976990", "155.934799 0.000000 -0.000000" );
      SpawnObject( "flip_apoc_table", "19711.099609 71.646599 819.734985", "0.000000 0.000000 -0.000000" );
      SpawnObject( "flip_satellite", "19747.500000 80.317001 689.104980", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_WhiteBoard", "19705.900391 72.192902 819.340027", "-172.632401 0.000000 -0.000000" );
      SpawnObject( "StaticObj_racks_crushed", "19702.900391 71.223999 819.437012", "-80.493568 0.000000 0.000000" );
      SpawnObject( "StaticObj_racks_broken", "19697.599609 71.199799 819.362000", "-47.100086 0.000000 0.000000" );
      SpawnObject( "StaticObj_classroom_case_b_crushed", "19699.800781 71.220901 820.773987", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "19700.400391 72.172600 823.966980", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "19702.300781 72.172600 823.036987", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "19702.300781 72.172600 825.763977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Table_Market", "19703.800781 71.680397 824.877991", "-92.532341 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_TirePile_Large", "19732.500000 71.458397 848.065002", "121.597946 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Ruin_Rubble", "19728.699219 71.436600 850.453003", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_Metal1", "19724.900391 71.694702 852.825989", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_Metal3", "19724.800781 71.827904 853.257019", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Medium2", "19724.500000 71.372803 855.338989", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Medium1", "19728.900391 71.497902 853.859009", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Large", "19729.400391 71.865196 854.546997", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Large", "19722.300781 71.986298 853.982971", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Large", "19724.000000 70.930801 849.533997", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Large", "19728.199219 71.490097 846.478027", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_PM_Forklift", "19727.000000 72.817200 840.692993", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_WoodBox1", "19722.400391 71.474998 827.450989", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_WoodBox1", "19721.300781 71.438301 827.559021", "25.331600 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19725.599609 71.399002 835.234985", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19725.800781 71.399002 835.705017", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19726.000000 71.399002 836.036987", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19726.199219 71.399002 836.333984", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19725.500000 71.399002 834.679016", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19725.400391 71.399002 835.041016", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19725.800781 71.399002 834.781982", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19726.099609 71.399002 835.223022", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19726.400391 71.399002 835.851990", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19726.500000 71.399002 836.575989", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19727.300781 71.399002 836.085022", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19725.099609 71.399002 835.351990", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19727.000000 71.399002 834.385986", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19727.699219 71.399002 835.179016", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19726.800781 71.399002 835.568970", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19726.800781 71.399002 834.632019", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19726.800781 71.399002 836.190002", "60.049271 0.000000 -0.000000" );
      SpawnObject( "Land_Metalbeam1", "19727.599609 71.399002 835.517029", "60.049271 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Container_2B", "19702.599609 73.694901 851.137024", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Container_2B", "19706.000000 73.694901 851.216980", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Container_2B", "19698.599609 73.694901 851.380005", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Moh", "19703.500000 72.375999 860.135010", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "19697.099609 72.375999 859.077026", "-89.366951 0.000000 0.000000" );
      SpawnObject( "Land_Container_1Bo", "19695.099609 72.375999 852.354980", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_garbage_container_glass", "19694.300781 72.040100 818.708008", "-157.624405 0.000000 -0.000000" );
      SpawnObject( "ANT_garbage_container_glass", "19694.300781 72.056801 820.508972", "179.206131 0.000000 -0.000000" );
      SpawnObject( "ANT_garbage_container_paper", "19694.400391 72.056801 832.956970", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_garbage_container_plastic", "19694.000000 72.056801 835.010010", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_garbage_container2_open", "19693.800781 71.888802 822.814026", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19683.500000 71.054703 840.174988", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19708.900391 71.106102 908.640991", "-49.164650 0.000000 0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19708.500000 71.013199 882.960022", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19732.599609 71.028397 905.393982", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_smaller_2", "19699.099609 70.290199 729.078003", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_smaller_2", "19699.099609 70.287102 710.245972", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_smaller_2", "19680.199219 70.296799 722.859985", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_smaller_2", "19680.199219 70.307701 710.293030", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_smaller_2", "19661.300781 70.313301 710.320984", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_smaller_2", "19661.300781 70.311798 722.872986", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19574.199219 71.301697 901.112000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "19556.400391 71.301697 901.112976", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19546.199219 71.725800 908.658020", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19546.199219 71.493500 905.572998", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19546.199219 71.605904 902.440979", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19546.199219 71.798302 899.481018", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19546.199219 71.710098 896.330994", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19546.199219 71.665001 894.534973", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19546.199219 71.619598 892.935974", "-2.907301 0.000000 0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19548.199219 71.548897 891.299011", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19551.199219 71.495102 891.293030", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19554.199219 71.580101 891.231018", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19557.199219 71.366096 891.146973", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19559.699219 71.240501 891.106018", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19562.199219 71.324501 891.080017", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19565.199219 71.223000 891.093018", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19568.199219 71.209000 891.056030", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19571.199219 71.165001 891.033020", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19574.500000 71.285599 891.026978", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19577.400391 71.282097 890.986023", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19580.500000 71.308601 890.916016", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19584.000000 71.268303 892.302002", "0.468611 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19584.000000 71.261902 894.815979", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19584.000000 71.158401 897.763000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19584.000000 71.288803 900.958008", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19584.000000 71.179802 904.070007", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19584.000000 71.238998 906.661987", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Roadblock_Bags_Long", "19584.000000 71.330101 909.426025", "-0.374188 0.000000 0.000000" );
      SpawnObject( "ANT_l_large_pile", "19583.400391 71.357803 891.304993", "29.556799 0.000000 -0.000000" );
      SpawnObject( "ANT_l_large_pile", "19582.199219 71.334702 891.268982", "-137.830536 0.000000 -0.000000" );
      SpawnObject( "Land_Misc_Greenhouse", "19551.099609 72.775398 896.450012", "179.762192 0.000000 -0.000000" );
      SpawnObject( "Land_Misc_Greenhouse", "19562.699219 72.727798 896.346008", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Misc_FireBarrel_Green", "19569.800781 72.543800 895.825989", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Misc_Well_Pump_Yellow", "19574.599609 72.198196 895.507019", "56.950089 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19629.800781 72.048401 787.921021", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19632.699219 72.023804 785.520020", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19666.900391 72.023598 802.291992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19664.400391 72.023598 801.543030", "38.000008 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets2", "19665.400391 71.709999 803.794983", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "19664.800781 71.608902 775.429016", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "19663.900391 71.608902 780.168030", "59.486691 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "19666.500000 71.608902 777.312988", "91.790558 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "19667.000000 71.608902 782.210999", "33.215565 0.000000 -0.000000" );
      SpawnObject( "StaticObj_ammoboxes_big", "19667.199219 72.304802 779.835022", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_ammoboxes_big", "19643.900391 72.264999 790.903015", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_ammoboxes_stacked", "19656.099609 71.989799 800.697998", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_ammoboxes_stacked", "19656.300781 71.982803 802.806030", "0.000000 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19603.400391 69.375000 838.312012", "-91.096420 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19603.099609 69.389397 857.044983", "89.104713 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19531.300781 69.380600 798.565979", "0.657461 0.000000 -0.000000" );
      SpawnObject( "maBLDS_Platform_small_2", "19532.199219 69.397003 840.244019", "1.783787 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Wreck_Trailer_Flat", "19573.199219 72.395798 844.979980", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_wreck_truck01_aban1_blue", "19580.000000 71.231598 850.424011", "24.206823 0.000000 -0.000000" );
      SpawnObject( "Land_wreck_truck01_aban1_blue", "19577.599609 71.158600 828.862000", "150.776657 0.000000 -0.000000" );
      SpawnObject( "Land_HA_Truck_5", "19548.300781 72.126801 844.408997", "-110.713409 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Wreck_UH1Y", "19563.800781 73.065697 798.492004", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Wreck_offroad02_aban2", "19560.099609 72.824303 813.234985", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Wreck_Trailer_Flat_DE", "19589.699219 72.394402 816.520996", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_wreck_digger", "19605.699219 74.386200 815.875977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_wreck_uaz", "19528.000000 72.873398 829.041992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_wreck_uaz", "19522.099609 72.879204 828.565002", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_wreck_ural", "19525.000000 73.309097 849.148987", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_wreck_v3s", "19531.000000 73.646004 849.114990", "171.042526 0.000000 -0.000000" );
      SpawnObject( "ANT_wreck_trailer_closed_base_prx", "19534.500000 72.012703 849.106018", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_wreck_trailer_closed", "19536.800781 72.992203 849.531006", "17.545265 0.000000 -0.000000" );
      SpawnObject( "Land_Garage_Row_Big", "19655.800781 74.579399 676.497986", "-175.918976 0.000000 -0.000000" );
      SpawnObject( "Land_Garage_Row_Small", "19649.699219 72.900902 708.728027", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "19592.599609 72.216499 816.960022", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "19594.800781 72.172600 817.521973", "-14.918162 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "19594.099609 72.172600 813.484009", "-53.667889 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19582.599609 71.973999 823.340027", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19581.099609 71.973999 840.049988", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19583.199219 71.973999 838.814026", "47.663071 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets2", "19581.400391 71.625900 837.570007", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets2", "19540.900391 71.660400 790.333008", "0.000000 0.000000 -0.000000" );
      SpawnObject( "flip_destroyed_bus", "19523.199219 73.611900 793.963989", "0.000000 0.000000 -0.000000" );
      SpawnObject( "land_jcbucket", "19525.300781 72.329399 787.625977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_rubbish_floor_1", "19528.500000 72.054901 789.414001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Garbage_Pile8", "19526.599609 72.558701 789.520996", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Garbage_Pile7", "19523.900391 72.207901 799.883972", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Garbage_Pile3", "19523.800781 72.283897 798.388977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_shelf_1side", "19524.300781 71.993103 805.559998", "123.943367 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_teacher_desk", "19524.099609 71.903297 803.711975", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_conference_table_a", "19524.099609 72.378799 802.015991", "-93.350182 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_canister_DZ", "19525.599609 72.313400 800.992004", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_canister_DZ", "19525.699219 72.313400 801.909973", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_canister_DZ", "19526.900391 72.313400 800.867981", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_debris_small_house", "19526.599609 72.026299 797.549011", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_dirtpile_small_house", "19520.699219 72.016899 802.651978", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_sawhorse_DZ", "19522.199219 71.984802 800.940979", "-69.149002 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_tools_racking_DZ", "19520.500000 71.988297 813.193970", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_Workbench", "19524.400391 71.914703 812.875000", "88.325081 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_Workbench", "19527.400391 71.977798 810.840027", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Aoh", "19531.099609 73.163503 808.380005", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Aoh", "19535.599609 73.163503 808.343994", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Garbage_Pile7", "19523.699219 72.475998 788.914978", "30.305376 0.000000 -0.000000" );
      SpawnObject( "Land_HA_Plane_5", "19593.699219 71.634300 865.650024", "-92.001938 0.000000 -0.000000" );
      SpawnObject( "Land_HA_Plane_5", "19603.500000 71.590202 865.622986", "-97.758987 0.000000 -0.000000" );
      SpawnObject( "Land_HA_Plane_5", "19590.400391 71.541901 846.874023", "-91.899193 0.000000 -0.000000" );
      SpawnObject( "Land_HA_Plane_5", "19604.800781 71.615799 847.205017", "-93.745735 0.000000 -0.000000" );
      SpawnObject( "Land_HA_Plane_5", "19590.099609 71.583397 827.981018", "-94.103409 0.000000 -0.000000" );
      SpawnObject( "Land_HA_Plane_5", "19605.300781 71.640701 829.325012", "-96.173042 0.000000 -0.000000" );
      SpawnObject( "Land_wood_palet_1", "19592.500000 71.200897 816.184021", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_wood_palet_1", "19592.599609 71.191002 817.588989", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_13", "19621.300781 70.605499 808.163025", "37.623753 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_locker_closed_blue_v1", "19672.699219 71.939903 869.921021", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_locker_closed_blue_v1", "19672.699219 71.942398 869.513977", "90.895279 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_locker_closed_blue_v1", "19672.699219 71.940201 869.122009", "88.584480 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_office_desk", "19672.599609 72.253304 867.750977", "-90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_office_desk", "19669.699219 72.253899 869.710999", "-178.072754 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_Desk_Globe", "19669.199219 72.880203 869.895996", "-178.072754 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_office_chair", "19669.699219 72.293503 869.276978", "-179.010269 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_office_chair", "19671.599609 72.302299 868.054993", "-28.052204 0.000000 0.000000" );
      SpawnObject( "land_flip_workbench_yellow", "19672.699219 72.676399 863.625000", "90.000000 0.000000 -0.000000" );
      SpawnObject( "land_flip_workbench_yellow", "19672.699219 72.670898 860.299988", "90.895279 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_Workbench", "19687.699219 71.879204 869.104004", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_Workbench", "19690.500000 71.819901 865.827026", "-0.094421 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_Workbench", "19678.400391 71.883598 866.247986", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_workbench_DZ", "19668.800781 72.455803 862.364014", "-90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_workbench_DZ", "19669.500000 72.455803 864.122986", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_ToolTrolley_02_F", "19684.199219 72.342697 869.072998", "-87.717422 0.000000 0.000000" );
      SpawnObject( "StaticObj_ToolTrolley_02_F", "19688.000000 72.352798 863.781982", "86.771706 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_tools_racking_DZ", "19680.599609 71.864799 869.062012", "-0.751889 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_tools_racking_DZ", "19679.500000 71.866898 869.052002", "-0.094421 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_tools_racking_DZ", "19686.400391 71.858498 865.130981", "-0.468611 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_tools_racking_DZ", "19685.300781 71.860497 865.109009", "-1.220565 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_tools_racking_DZ", "19684.199219 71.861198 865.078979", "-1.405958 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_tools_racking_DZ", "19683.099609 71.856903 865.056030", "-1.031687 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_bucket", "19678.800781 71.832100 863.424988", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_box_c_multi", "19686.500000 71.859398 866.502991", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_metalcrate", "19678.300781 71.825203 863.057007", "-0.094421 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_metalcrate_02", "19679.199219 71.859703 862.888000", "88.903564 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_box_c", "19678.400391 72.962601 864.481018", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_box_c", "19684.599609 72.137100 866.984009", "61.452103 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_box_c_multi", "19668.199219 71.878197 859.390015", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_pc", "19672.599609 72.665398 867.489014", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_pc", "19670.000000 72.666702 869.866028", "-77.684990 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_radio_b", "19668.500000 72.934898 864.364990", "100.758751 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_vending_machine", "19666.800781 72.992500 869.525024", "-2.158194 0.000000 0.000000" );
      SpawnObject( "StaticObj_Furniture_blackboard", "19681.800781 71.683502 865.758972", "-130.324631 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_Drill", "19685.800781 73.073700 868.924988", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_grinder", "19688.900391 72.808098 869.004028", "1.122627 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_Metal_cutting_saw", "19690.400391 72.921501 867.622009", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_soustruh_proxy", "19670.599609 72.637901 857.853027", "90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_Vise", "19690.300781 72.595802 863.844971", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_Vise", "19690.300781 72.595802 864.393982", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Furniture_wheel_cart_DZ", "19674.099609 71.204201 868.489990", "-17.453201 0.000000 0.000000" );
      SpawnObject( "Land_Container_1Aoh", "19672.099609 72.360703 878.883972", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Aoh", "19669.099609 72.331902 878.859985", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_BoardsPack2", "19663.500000 71.910896 877.000977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_BoardsPack2", "19663.699219 71.910896 880.797974", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_BoardsPack2", "19666.199219 71.910896 878.818970", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_ConcretePanels", "19660.800781 71.905296 877.304993", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_ConcretePanels", "19680.599609 71.944397 857.499023", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_ConcretePanels", "19684.099609 71.957100 857.336975", "-90.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Pallets3", "19660.599609 71.972801 880.216003", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_TirePile", "19677.599609 75.453400 857.455017", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_TirePile_Group", "19679.099609 75.359703 856.976013", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Medium1", "19683.900391 71.545799 855.193970", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Medium1", "19667.199219 71.485100 882.851990", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Medium2", "19669.099609 71.436699 882.789978", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_Wood2", "19669.900391 70.862503 884.681030", "104.515778 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_Wood3", "19660.500000 73.199402 819.744995", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Hotel_Damaged_Rubble", "19581.599609 65.240601 830.289001", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_Bricks1", "19565.099609 71.217400 837.638000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_Bricks1", "19567.300781 71.474098 821.473022", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_concrete2", "19567.000000 70.877296 836.851990", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_concrete2", "19573.699219 71.105003 822.487000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_large", "19571.400391 71.945198 809.255005", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_large", "19596.900391 71.670898 815.085022", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_medium2", "19597.599609 71.489502 818.710022", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_medium2", "19569.599609 71.300301 823.867004", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_medium2", "19570.500000 71.106903 823.164001", "105.370193 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_medium2", "19566.900391 71.133797 836.200989", "90.633041 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_medium2", "19571.599609 71.248703 846.297974", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_large", "19572.500000 71.654297 848.622986", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_large", "19578.199219 71.471298 849.067993", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_large", "19676.199219 71.891403 889.132996", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rubble_dirtpile_large", "19673.699219 70.959396 886.247009", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Water_Station", "19701.099609 72.855003 742.539978", "-178.267090 0.000000 -0.000000" );
      SpawnObject( "flip_watertower", "19694.099609 74.494202 744.786011", "42.408867 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "19722.400391 72.325302 840.971985", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "19719.099609 72.333900 849.526001", "133.512863 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "19714.599609 72.366997 829.822998", "-116.154945 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big4", "19656.400391 72.446800 794.448975", "102.262962 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19604.099609 68.193604 864.518005", "-0.563035 0.000000 0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19604.099609 68.193604 867.145020", "-0.563035 0.000000 0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19594.400391 68.193604 866.999023", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19594.300781 68.193604 864.379028", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19590.199219 68.193604 866.841003", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19590.199219 68.193604 864.434998", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19604.500000 68.193604 845.625000", "-1.688831 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19604.500000 68.193604 848.106995", "-0.375295 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19594.699219 68.193604 845.624023", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19594.800781 68.193604 848.244019", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19590.300781 68.193604 845.458008", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19590.500000 68.193604 848.004028", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19591.599609 68.193604 827.140015", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19592.000000 68.193604 829.609009", "-1.501200 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19600.800781 68.193604 827.375977", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19601.000000 68.193604 829.786011", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19604.699219 68.193604 827.564026", "-0.656747 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Platform2_Block", "19604.699219 68.193604 829.802002", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Medium2", "19826.500000 4.921050 650.310974", "-50.383877 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_GasMeterCabinet", "19702.099609 71.881798 898.536011", "-90.259315 0.000000 -0.000000" );
      SpawnObject( "Land_wreck_truck01_aban2_firetruck", "19699.599609 71.193199 713.901978", "87.538292 0.000000 -0.000000" );
      SpawnObject( "Land_wreck_truck01_aban2_firetruck", "19699.300781 71.198196 707.325989", "86.975655 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "4369.140137 208.134003 4596.649902", "-0.656746 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "4352.430176 208.082001 4597.149902", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "4341.310059 207.893005 4599.979980", "13.604602 0.000000 -0.000000" );
      SpawnObject( "Land_FakeHillSide_17", "4332.569824 207.929993 4603.649902", "0.000000 0.000000 -0.000000" );
      //// -- Shatwest_Latina.txt (6 objects)
      SpawnObject("Land_shatwest_signage", "17550.599609 155.033005 4154.089844", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_shatwest_signage", "17541.800781 153.492004 4155.689941", "89.104713 0.000000 -0.000000");
      SpawnObject("Land_shatwest_signage", "17041.900391 154.369003 4031.169922", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_shatwest_signage", "17025.599609 154.326004 4037.550049", "90.000000 0.000000 -0.000000");
      SpawnObject("Land_shatwest_signage", "17035.199219 154.039993 4048.620117", "89.366951 0.000000 -0.000000");
      SpawnObject("Land_shatwest_signage", "17550.599609 153.759003 4160.439941", "0.000000 0.000000 -0.000000");
      //// -- ROAD BARRICADE BYPASS TILSLEY.txt (19 objects)
      SpawnObject("StaticObj_Ruin_Rubble", "17849.300781 76.975899 1899.260010", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Hotel_Damaged_Rubble", "17848.800781 69.673500 1902.089966", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_bricks3", "17855.699219 75.498100 1897.219971", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_dirtpile_large", "17853.300781 77.466797 1903.449951", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_dirtpile_medium1", "17850.500000 77.208000 1907.930054", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_dirtpile_medium2", "17849.599609 76.792900 1904.479980", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_metal2", "17852.599609 77.691002 1901.479980", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_rocks1", "17885.400391 76.745102 1868.910034", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_rocks2", "17875.599609 71.754601 1867.130005", "75.723320 0.000000 -0.000000");
      SpawnObject("ANT_rubble_wood1", "17872.500000 73.333504 1875.369995", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_wood2", "17873.300781 72.532700 1871.719971", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_wood3", "17871.699219 73.702904 1870.479980", "0.000000 5.000141 0.000000");
      SpawnObject("ANT_ruin_rubble", "17882.400391 72.974197 1879.619995", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_dirtpile_large", "17878.000000 73.291496 1878.800049", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Bricks1", "17877.900391 71.737503 1875.130005", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Bricks2", "17876.000000 70.717102 1863.849976", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Bricks1", "17873.800781 70.455399 1862.709961", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_wreck_bulldozer", "17851.000000 77.129402 1892.050049", "0.000000 6.001426 6.001426");
      SpawnObject("ANT_wreck_digger", "17874.199219 76.512299 1878.650024", "16.999998 -5.000000 4.000000");
      //// -- racetrack.txt (24 objects)
      SpawnObject("Land_groggs_signage", "19066.300781 25.879299 8680.299805", "-179.485916 0.000000 -0.000000");
      SpawnObject("Land_groggs_signage2", "19074.199219 25.877899 8680.320313", "-179.202637 0.000000 -0.000000");
      SpawnObject("Land_chickenslut_signage", "19132.900391 25.878099 8679.799805", "-179.104706 0.000000 -0.000000");
      SpawnObject("Land_chickenslut_signage", "19140.800781 25.899200 8679.719727", "-179.300552 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_down", "19160.099609 26.866800 8676.750000", "8.257126 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_down", "19161.199219 26.887600 8676.780273", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_down", "19156.400391 27.038300 8676.969727", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_torycarvery_signage", "19161.699219 27.525000 8676.769531", "179.300552 0.000000 -0.000000");
      SpawnObject("flip_watchtower", "19213.500000 49.988899 8396.650391", "3.282034 0.000000 -0.000000");
      SpawnObject("Land_Radio_PanelPAS", "19215.099609 53.420502 8396.500000", "3.282034 0.000000 -0.000000");
      SpawnObject("flip_watchtower", "19136.699219 46.490898 8389.139648", "1.783787 0.000000 -0.000000");
      SpawnObject("Land_chickenslut_signage", "19390.599609 25.869801 8475.839844", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_chickenslut_signage", "19390.599609 25.851500 8467.889648", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_groggs_signage", "19390.500000 25.882799 8409.139648", "-89.366951 0.000000 0.000000");
      SpawnObject("Land_groggs_signage2", "19390.500000 25.876600 8401.219727", "-91.096420 0.000000 -0.000000");
      SpawnObject("Land_Misc_DeerStand1", "19216.099609 47.049801 8408.730469", "-179.011673 0.000000 -0.000000");
      SpawnObject( "StaticObj_Tank_Medium", "19633.500000 71.989998 885.028992", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Tank_Small_Gas", "19640.099609 71.895203 885.020996", "89.366951 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Tisy_AirShaft", "19643.199219 73.464699 884.401978", "179.580338 0.000000 -0.000000" );
      SpawnObject( "Sewer_Drain_Junction_Platform_Door_Connection", "19644.800781 71.167198 884.414001", "-91.096420 0.000000 -0.000000" );
      SpawnObject( "vlad_wirebox", "19650.099609 71.679703 886.284973", "-0.563035 0.000000 0.000000" );
      SpawnObject( "land_vlad_gas_station", "19635.099609 73.519302 863.031982", "-0.099595 0.000000 -0.000000" );
      SpawnObject( "StaticObj_FueldStation_Feed_NoFuel", "19631.500000 71.252403 862.846008", "-0.099595 0.000000 -0.000000" );
      SpawnObject( "StaticObj_FueldStation_Feed_NoFuel", "19641.099609 71.259499 862.666016", "-0.099595 0.000000 -0.000000" );
      SpawnObject( "ANT_fuelstation_feed", "19631.500000 72.082001 857.857971", "-0.099595 0.000000 -0.000000" );
      SpawnObject( "ANT_fuelstation_feed", "19641.099609 72.053001 857.877014", "-0.099595 0.000000 -0.000000" );
      //// -- notto_latina.txt (5 objects)
      SpawnObject("ANT_wooden_planks_down", "17512.300781 153.496994 4285.250000", "50.289913 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_down", "17512.400391 153.490005 4285.319824", "50.383057 0.000000 -0.000000");
      SpawnObject("Land_notto_signage", "17509.699219 155.298996 4283.970215", "72.998840 0.000000 -0.000000");
      SpawnObject("Land_notto_signage", "17511.599609 155.429993 4289.009766", "140.547577 0.000000 -0.000000");
      SpawnObject("Land_notto_signage", "17528.099609 155.404999 4302.709961", "140.361649 0.000000 -0.000000");
      //// -- nasper-special climbover.txt (2 objects)
      SpawnObject("Land_Castle_Stairs", "9603.889648 165.108002 3247.949951", "107.427269 0.000000 -0.000000");
      SpawnObject("Land_Castle_Stairs", "9600.389648 165.024002 3235.580078", "14.730431 0.000000 -0.000000");
      //// -- nasper high street 1.txt (19 objects)
      SpawnObject("Land_ladsbroke_signage", "9203.570313 153.766006 2723.760010", "179.951035 0.000000 -0.000000");
      SpawnObject("Land_poots_signage", "9233.820313 153.522995 2723.689941", "179.769196 0.000000 -0.000000");
      SpawnObject("Land_wtfsmith_signage", "9278.450195 153.276993 2723.649902", "179.856613 0.000000 -0.000000");
      SpawnObject("Land_hti_signage", "9338.860352 153.501999 2737.790039", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_up", "9137.150391 152.278000 2747.689941", "88.733902 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_up", "9137.110352 152.313004 2747.629883", "89.104713 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "9137.150391 154.028000 2747.689941", "-90.633041 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "9136.679688 155.565994 2744.320068", "-0.657461 0.000000 0.000000");
      SpawnObject("Land_tosco_signage", "9114.080078 155.475006 2744.050049", "-1.031687 0.000000 0.000000");
      SpawnObject("Land_argod_signage", "9451.919922 153.233002 2556.979980", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_poondland_signage", "9451.889648 153.841003 2573.790039", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_shatwest_signage", "9513.120117 152.710999 2662.600098", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_shatwest_signage", "9511.950195 159.983994 2653.639893", "-0.188841 0.000000 0.000000");
      SpawnObject("Land_torycarvery_signage", "9451.870117 153.822006 2682.209961", "-44.565258 0.000000 0.000000");
      SpawnObject("Land_pissitsmonday_signage", "9430.040039 153.419998 2680.979980", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_notto_signage", "9400.769531 153.382004 2681.020020", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_groggs_signage2", "9612.750000 153.490005 2561.830078", "0.657461 0.000000 -0.000000");
      SpawnObject("Land_chickenslut_signage", "9619.450195 153.490005 2561.780029", "0.563035 0.000000 -0.000000");
      SpawnObject("Land_specsavedya_signage", "9740.080078 153.322998 2618.459961", "90.000000 0.000000 -0.000000");
      //// -- main_road_fixes.txt (121 objects)
      SpawnObject("StaticObj_Bridge_Wood_50", "7858.000000 69.513100 1910.380005", "17.449537 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_DirtPile_Large", "7962.410156 147.001999 1954.479980", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "7847.029785 142.000000 1901.000000", "62.999790 1.000209 0.017655");
      SpawnObject("Land_Pier_Floating_Bridge", "7821.560059 140.539001 1888.030029", "63.002338 5.000141 -0.087642");
      SpawnObject("Land_Pier_Floating_Bridge", "7805.009766 139.563004 1879.510010", "62.997292 2.000723 0.007287");
      SpawnObject("Land_Pier_Floating_Bridge", "7512.299805 116.455002 1728.599976", "62.998116 6.001426 2.023645");
      SpawnObject("StaticObj_Bridge_Wood_50", "7404.819824 104.174004 1673.109985", "-27.499460 1.000209 -5.006962");
      SpawnObject("StaticObj_Bridge_Wood_50", "7374.930176 91.765701 1657.439941", "-28.001385 1.000209 -5.007003");
      SpawnObject("StaticObj_Bridge_Wood_50", "7343.680176 97.994904 1641.260010", "-26.875788 2.000723 -6.010301");
      SpawnObject("Land_Pier_Floating_Bridge", "7194.910156 85.748100 1516.640015", "32.650524 2.000723 0.003281");
      SpawnObject("Land_Pier_Floating_Bridge", "7180.819824 84.739304 1494.479980", "32.281406 2.000723 0.002811");
      SpawnObject("Land_Pier_Floating_Bridge", "7166.759766 83.806503 1472.589966", "33.000301 2.000723 0.003030");
      SpawnObject("StaticObj_Bridge_Wood_50", "7374.990234 101.254997 1657.469971", "-27.999472 2.000722 -4.989278");
      SpawnObject("StaticObj_Bridge_Wood_50", "7310.620117 93.691399 1624.540039", "-26.999813 1.000209 -7.011325");
      SpawnObject("Land_Pier_Floating_Bridge", "7152.770020 82.702797 1451.209961", "33.187466 2.998345 -0.060569");
      SpawnObject("StaticObj_Bridge_Stone_25", "6586.189941 71.174698 1389.089966", "-72.096313 3.000000 0.104812");
      SpawnObject("StaticObj_Bridge_Stone_25", "6562.810059 72.047501 1396.689941", "-72.000008 1.000000 -0.000000");
      SpawnObject("StaticObj_Bridge_Stone_25", "6539.529785 71.829102 1404.040039", "-73.000008 -2.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "4291.169922 197.375000 6071.939941", "87.028740 -2.000723 -0.033367");
      SpawnObject("Land_Pier_Floating_Bridge", "4312.700195 196.628006 6072.819824", "-92.003746 2.000723 0.067153");
      SpawnObject("Land_Pier_Floating_Bridge", "4334.609863 196.235001 6073.600098", "-91.951752 1.000209 -0.034062");
      SpawnObject("Land_Pier_Floating_Bridge", "4359.850098 195.843002 6074.700195", "-92.936050 1.000209 0.449439");
      SpawnObject("Land_Pier_Floating_Bridge", "4524.930176 195.904007 6082.629883", "-92.003746 -2.000723 -0.067153");
      SpawnObject("Land_Pier_Floating_Bridge", "4548.370117 196.363007 6083.560059", "87.168686 0.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "4848.229980 214.656006 6137.620117", "-0.279766 0.000000 0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "4846.830078 214.453995 6163.819824", "-6.343589 -1.000209 -0.034251");
      SpawnObject("Land_Pier_Floating_Bridge", "4793.779785 216.447006 6560.600098", "-7.002103 -1.000209 -0.013379");
      SpawnObject("Land_Pier_Floating_Bridge", "4784.560059 217.046997 6628.779785", "-7.250659 3.000000 0.006616");
      SpawnObject("Land_Pier_Floating_Bridge", "4781.799805 218.237000 6650.770020", "-7.031781 2.998346 0.007373");
      SpawnObject("Land_Pier_Floating_Bridge", "4778.209961 219.431000 6677.370117", "-8.623026 2.000723 0.000656");
      SpawnObject("Land_Pier_Floating_Bridge", "4796.819824 216.240005 6537.500000", "-7.845180 2.000723 -0.000439");
      SpawnObject("Land_Pier_Floating_Bridge", "14428.400391 146.919006 1926.780029", "88.194656 0.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "14457.400391 146.134995 1927.739990", "-91.938148 3.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "14479.200195 145.177994 1928.510010", "87.999992 -2.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "14505.000000 143.035995 1929.530029", "87.000000 -8.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "14651.500000 137.365005 1935.609985", "87.913528 0.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "14712.799805 137.354996 1938.069946", "88.007675 0.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "15950.400391 132.190994 1987.030029", "87.996239 -2.000723 -0.909720");
      SpawnObject("Land_Pier_Floating_Bridge", "15975.500000 130.121002 1987.819946", "87.743416 -7.999250 -1.008741");
      SpawnObject("Land_Pier_Floating_Bridge", "16001.400391 127.452003 1988.839966", "87.500107 -2.998345 0.130717");
      SpawnObject("StaticObj_Rubble_Concrete1", "15974.400391 128.580994 1986.260010", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Concrete2", "15968.599609 128.276001 1982.849976", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Concrete1", "15970.000000 129.570999 1987.869995", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Target_Support", "15962.400391 130.406006 1990.310059", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Target_Support", "15962.500000 130.787994 1984.339966", "-173.945816 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_DirtPile_Medium1", "15975.599609 128.505997 1989.930054", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_DirtPile_Medium2", "15974.400391 128.479004 1989.030029", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Misc_TirePile_Large", "15973.099609 128.582001 1992.449951", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Furniture_dirtpile_small_house", "15969.799805 128.626007 1996.780029", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_dirtpile_small", "15971.900391 128.641998 1998.089966", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_dirtpile_large", "15971.700195 128.944000 1995.849976", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Garbage_Pile4", "15965.500000 129.123001 1984.729980", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Garbage_Pile7", "15964.900391 129.240997 1987.770020", "-88.733902 0.000000 0.000000");
      SpawnObject("StaticObj_Garbage_Pile5", "15960.299805 130.244003 1986.739990", "0.000000 1.000209 18.000248");
      SpawnObject("StaticObj_Garbage_Pile6", "15962.799805 129.587997 1985.180054", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Garbage_Pile2", "15963.400391 129.309998 1987.339966", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Garbage_Pile3", "15962.099609 129.460999 1988.640015", "0.000000 0.000000 2.998267");
      SpawnObject("StaticObj_Furniture_dirtpile_small_house", "15972.799805 127.989998 1982.520020", "0.000000 5.000000 -0.000000");
      SpawnObject("ANT_garbage_pile1", "15973.299805 128.362000 1982.880005", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_farm_manurepile", "15977.400391 127.981003 1985.040039", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_farm_manurepile", "15976.900391 128.662994 1993.569946", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_farm_manurepile", "15979.700195 128.541000 1990.930054", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_farm_manurepile", "15979.599609 128.423004 1994.709961", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_farm_manurepile", "15983.000000 128.005005 1990.189941", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_farm_manurepile", "15981.900391 128.483002 1985.839966", "91.573029 0.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "19006.699219 89.314003 5771.149902", "5.001671 5.000141 0.000531");
      SpawnObject("Land_Pier_Floating_Bridge", "19004.099609 85.711800 5744.970215", "6.001509 9.999815 0.001818");
      SpawnObject("Land_Pier_Floating_Bridge", "19002.099609 82.631897 5722.189941", "4.498488 6.001425 0.001781");
      SpawnObject("Land_Pier_Floating_Bridge", "19009.000000 91.005302 5796.649902", "5.656878 2.000723 0.001465");
      SpawnObject("Land_Pier_Floating_Bridge", "19000.500000 80.816399 5704.479980", "5.342007 6.001426 0.002325");
      SpawnObject("Land_Pier_Floating_Bridge", "18998.199219 78.640503 5679.540039", "5.279889 4.000385 0.001010");
      SpawnObject("StaticObj_Target_Support", "19010.599609 87.820801 5766.609863", "79.838364 0.000000 -0.000000");
      SpawnObject("StaticObj_Target_Support", "19001.099609 87.502197 5764.589844", "-96.551720 0.000000 -0.000000");
      SpawnObject("StaticObj_Target_Support", "18999.199219 84.699898 5747.470215", "-79.759377 0.000000 0.000000");
      SpawnObject("StaticObj_Target_Support", "19008.099609 84.552498 5745.209961", "83.603325 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Bricks2", "19004.900391 86.900299 5761.560059", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Hotel_Damaged_Rubble", "19010.900391 79.906303 5762.339844", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Rocks2", "18998.800781 86.971001 5764.459961", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Wood3", "18999.400391 86.222000 5757.310059", "-53.102654 7.999248 0.017714");
      SpawnObject("StaticObj_Ruin_Rubble", "19005.300781 84.684998 5757.819824", "61.827572 0.000000 -0.000000");
      SpawnObject("ANT_rubble_bricks3", "19011.500000 85.352303 5756.750000", "1.315009 0.000000 4.003492");
      SpawnObject("ANT_rubble_bricks3", "19006.500000 83.912598 5756.049805", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_bricks3", "19001.099609 85.233704 5759.640137", "72.341385 0.000000 -0.000000");
      SpawnObject("ANT_rubble_rocks1", "19000.000000 80.656898 5749.979980", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_metal1", "19001.199219 83.533699 5745.850098", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_rubble_metal3", "19002.099609 82.946198 5746.830078", "25.428366 0.000000 -0.000000");
      SpawnObject("ANT_rubble_dirtpile_large", "18999.900391 83.560699 5740.870117", "0.000000 9.999813 1.000209");
      SpawnObject("Land_Pier_Floating_Bridge", "13661.200195 166.218002 5481.089844", "89.530693 7.000000 -3.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "13687.299805 169.192993 5481.259766", "90.000000 6.000000 -2.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "13710.400391 171.382996 5481.180176", "90.782478 4.999998 -1.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "13736.599609 173.507004 5480.990234", "90.029297 3.999999 -1.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "13796.099609 178.468002 5480.799805", "90.000000 3.000000 -1.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "13821.000000 179.880005 5480.839844", "90.000000 3.000000 -1.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "13847.400391 180.891006 5480.870117", "89.938187 1.000000 0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "13871.500000 180.535004 5480.799805", "90.634293 -3.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "13897.200195 179.630997 5480.609863", "90.259315 0.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "10575.500000 112.054001 1544.680054", "-89.967857 -9.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "10575.500000 111.802002 1535.349976", "-89.967773 -7.999997 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "10598.500000 115.028000 1535.260010", "90.634560 7.999997 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "10599.000000 115.251999 1544.630005", "90.000000 7.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "10625.299805 118.786003 1535.150024", "-89.750481 -6.999999 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "10625.599609 118.615997 1544.400024", "-88.999992 -8.000000 -0.000000");
      SpawnObject("ANT_rubble_concrete1", "16726.199219 154.983002 5144.270020", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "16331.500000 164.061996 5102.569824", "0.000000 3.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "16331.500000 162.423996 5076.689941", "0.000000 4.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "16331.500000 161.212006 5052.129883", "0.000000 2.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "16331.500000 159.778000 5027.339844", "0.000000 4.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "16331.500000 158.649002 5005.430176", "0.000000 2.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "16331.500000 157.945999 4982.229980", "0.000000 2.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "16331.500000 156.753006 4956.430176", "0.000000 3.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "18965.800781 78.639900 5325.350098", "5.441814 -3.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "18963.300781 79.226997 5300.609863", "5.970280 0.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "19013.599609 112.349998 5958.060059", "-16.846798 9.999996 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "19005.699219 117.453003 5983.950195", "162.784088 -10.999998 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "18999.099609 120.987999 6005.359863", "-17.343216 6.999999 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "18992.300781 124.435997 6026.899902", "-17.469076 9.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "18988.199219 126.414001 6039.979980", "-18.000000 7.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "18975.900391 131.578995 6079.560059", "-16.844398 6.999999 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "18968.300781 134.580994 6104.430176", "-17.281456 6.000000 -0.000000");
      SpawnObject("Land_Pier_Floating_Bridge", "18960.500000 137.274994 6129.359863", "-17.501123 4.999998 -0.000000");
      SpawnObject("PSD_treeman", "18951.199219 156.526001 6501.839844", "0.000000 0.000000 -0.000000");
      //// -- MAIN ROAD ACCESSORIES.txt (9 objects)
      SpawnObject("ANT_sign_attention", "4796.509766 216.666000 6487.810059", "-30.774311 0.000000 0.000000");
      SpawnObject("ANT_sign_attention", "4779.000000 223.018997 6724.890137", "175.918976 0.000000 -0.000000");
      SpawnObject("ANT_sign_attention", "4791.709961 215.018005 6058.149902", "67.174690 0.000000 -0.000000");
      SpawnObject("ANT_sign_attention", "4850.189941 215.524002 6192.160156", "154.152710 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Concrete1", "4940.689941 213.727997 6821.660156", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Concrete2", "4942.250000 211.865997 6823.890137", "0.000000 -3.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Concrete1", "4892.470215 226.091995 6781.259766", "179.861984 0.000000 -4.999999");
      SpawnObject("StaticObj_Rubble_DirtPile_Large", "4897.259766 224.363998 6780.919922", "10.853988 -3.000000 15.999994");
      SpawnObject("StaticObj_Rubble_Concrete1", "4894.959961 224.197998 6783.970215", "0.000000 0.000000 -0.000000");
      //// -- lowridge.txt (2 objects)
      SpawnObject("Land_City_Stand_Grocery", "4431.270020 210.160004 4785.250000", "33.589260 0.000000 -0.000000");
      SpawnObject("Land_groggs_signage2", "4428.979980 211.878006 4781.680176", "33.776894 0.000000 -0.000000");
      //// -- hydro well.txt (1 objects)
      SpawnObject("Land_Misc_Well_Pump_Blue", "3362.159912 10.323700 3706.030029", "81.522835 0.000000 -0.000000");
      //// -- hydro east.txt (5 objects)
      SpawnObject("Land_chickenslut_signage", "3964.030029 14.407100 3661.000000", "-38.000008 0.000000 0.000000");
      SpawnObject("Land_groggs_signage2", "3953.179932 14.396400 3652.600098", "-38.373741 0.000000 0.000000");
      SpawnObject("Land_torycarvery_signage", "3958.479980 13.936100 3657.139893", "-38.093262 0.000000 0.000000");
      SpawnObject("ANT_wooden_planks_up", "3980.060059 12.655600 3639.300049", "52.072231 0.000000 -0.000000");
      SpawnObject("Land_notto_signage", "3979.969971 14.621000 3639.300049", "52.354084 0.000000 -0.000000");
      //// -- highrise entry nasper.txt (6 objects)
      SpawnObject("StaticObj_Power_Pole_Wood6_H", "9596.139648 208.716003 3240.070068", "74.664505 -78.000443 0.020732");
      SpawnObject("StaticObj_Power_Pole_Wood6_H", "9596.790039 208.701004 3241.370117", "-99.456146 78.000481 0.006218");
      SpawnObject("StaticObj_Power_Pole_Wood5_rus", "9595.660156 208.455994 3240.649902", "-101.380142 75.995232 179.901138");
      SpawnObject("StaticObj_Rubble_DirtPile_Medium2", "9591.769531 207.796005 3239.449951", "93.798904 0.000000 -0.000000");
      SpawnObject("ANT_castle_stairs", "9530.040039 166.925995 3320.320068", "-177.417740 0.000000 -0.000000");
      SpawnObject("ANT_castle_stairs", "9512.639648 173.067001 3321.159912", "15.199542 0.000000 -0.000000");
      //// -- groggs_vladton.txt (8 objects)
      SpawnObject("Land_groggs_signage", "17686.000000 34.473499 8550.980469", "-44.095932 0.000000 0.000000");
      SpawnObject("Land_poots_signage", "17976.900391 25.708000 8297.440430", "-89.977905 0.000000 -0.000000");
      SpawnObject("Land_poondland_signage", "17976.800781 25.860901 8260.519531", "-89.977364 0.000000 -0.000000");
      SpawnObject("Land_argod_signage", "17976.699219 25.894699 8251.900391", "-90.634560 0.000000 -0.000000");
      SpawnObject("Land_shatwest_signage", "17991.800781 26.265600 8199.089844", "89.039574 0.000000 -0.000000");
      SpawnObject("Land_ladsbroke_signage", "17740.000000 25.831301 7995.669922", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_groggs_signage2", "17801.900391 25.533600 7984.430176", "-179.575806 0.000000 -0.000000");
      SpawnObject("Land_notto_signage", "17580.500000 36.045799 8386.660156", "134.543579 0.000000 -0.000000");
      //// -- groggs_upper_rosco.txt (1 objects)
      SpawnObject("Land_groggs_signage", "12925.500000 153.628998 6382.479980", "44.472939 0.000000 -0.000000");
      //// -- groggs_tinside.txt (8 objects)
      SpawnObject("Land_groggs_signage", "5113.430176 153.233002 1904.939941", "162.502792 0.000000 -0.000000");
      SpawnObject("Land_ladsbroke_signage", "5094.750000 153.205002 1935.750000", "-108.928719 0.000000 -0.000000");
      SpawnObject("Land_specsavedya_signage", "5086.290039 152.694000 1896.390015", "162.689697 0.000000 -0.000000");
      SpawnObject("Land_argod_signage", "5071.029785 152.699005 1891.469971", "162.315689 0.000000 -0.000000");
      SpawnObject("Land_shatwest_signage", "5057.930176 152.792007 1903.489990", "-17.732021 0.000000 0.000000");
      SpawnObject("Land_poondland_signage", "5043.620117 153.063995 1883.060059", "162.499130 0.000000 -0.000000");
      SpawnObject("Land_wtfsmith_signage", "5140.830078 152.807007 1913.380005", "162.128403 0.000000 -0.000000");
      SpawnObject("Land_hti_signage", "5207.129883 153.328003 1932.109985", "-148.242096 0.000000 -0.000000");
      //// -- groggs_riker_south.txt (13 objects)
      SpawnObject("Land_groggs_signage", "12574.599609 44.873699 1328.189941", "-0.842821 0.000000 0.000000");
      SpawnObject("Land_argod_signage", "12518.099609 44.597099 1327.449951", "-1.315009 0.000000 0.000000");
      SpawnObject("Land_wtfsmith_signage", "12534.099609 44.646702 1327.709961", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_ladsbroke_signage", "12589.299805 45.059799 1328.390015", "-0.842821 0.000000 0.000000");
      SpawnObject("ANT_wooden_planks_up", "12817.099609 45.813499 1326.280029", "-2.438184 0.000000 0.000000");
      SpawnObject("ANT_wooden_planks_up", "12817.099609 45.804699 1326.310059", "-3.001848 0.000000 0.000000");
      SpawnObject("Land_notto_signage", "12817.099609 47.570999 1326.160034", "-2.627210 0.000000 0.000000");
      SpawnObject("Land_notto_signage", "12813.700195 49.106201 1326.439941", "87.240410 0.000000 -0.000000");
      SpawnObject("Land_notto_signage", "12812.599609 49.038601 1349.290039", "86.771706 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "12708.200195 49.242401 1390.770020", "-14.073360 0.000000 0.000000");
      SpawnObject("Land_hti_signage", "12716.400391 48.905399 1392.790039", "-13.885963 0.000000 0.000000");
      SpawnObject("Land_torycarvery_signage", "12658.400391 44.744999 1330.010010", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_pissitsmonday_signage", "12711.400391 44.920300 1330.660034", "-0.751889 0.000000 0.000000");
      //// -- groggs_nasper.txt (1 objects)
      SpawnObject("Land_groggs_signage", "9237.389648 153.615997 2508.729980", "179.761642 0.000000 -0.000000");
      //// -- groggs_latina_south.txt (22 objects)
      SpawnObject("Land_groggs_signage", "17727.000000 153.641998 4021.510010", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_ladsbroke_signage", "17649.699219 153.843994 4021.580078", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_hti_signage", "17752.400391 153.401001 4021.429932", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_poots_signage", "17768.300781 153.695007 4021.560059", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "17331.599609 153.679993 4021.350098", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_poondland_signage", "17352.699219 153.667999 4021.409912", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_wtfsmith_signage", "17378.099609 153.417007 4021.330078", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_groggs_signage2", "17666.199219 153.462006 4200.120117", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_specsavedya_signage", "17741.400391 153.613998 4021.449951", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_specsavedya_signage", "17383.599609 152.306000 4021.449951", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_argod_signage", "17758.099609 153.389008 3916.340088", "-179.667755 0.000000 -0.000000");
      SpawnObject("Land_torycarvery_signage", "17770.699219 153.285004 3916.270020", "179.954529 0.000000 -0.000000");
      SpawnObject("Land_chickenslut_signage", "17666.199219 153.464005 4179.220215", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_pissitsmonday_signage", "17666.300781 153.417999 4140.540039", "-90.895279 0.000000 -0.000000");
      SpawnObject("Land_notto_signage", "17677.900391 153.662994 4031.649902", "-179.951035 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "17699.099609 153.658005 4031.550049", "-179.951035 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_up", "17757.900391 154.255997 4344.930176", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "17757.900391 155.957993 4344.979980", "-179.674774 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "17761.400391 157.483002 4344.569824", "-90.000000 0.000000 0.000000");
      SpawnObject("Land_tosco_signage", "17760.000000 157.445999 4320.660156", "0.094421 0.000000 -0.000000");
      SpawnObject("Land_groggs_signage2", "17568.000000 153.460999 4218.490234", "-0.469119 0.000000 -0.000000");
      SpawnObject("Land_chickenslut_signage", "17539.000000 153.455994 4218.379883", "-0.750579 0.000000 -0.000000");
      //// -- groggs_flipton.txt (1 objects)
      SpawnObject("Land_groggs_signage", "7528.430176 159.595993 4006.360107", "162.503448 0.000000 -0.000000");
      //// -- Groggs.txt (20 objects)
      SpawnObject("Land_groggs_signage", "17460.300781 153.412994 3587.709961", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_groggs_signage", "17462.199219 152.876007 3591.070068", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_groggs_signage", "17461.800781 152.860992 3591.120117", "90.000000 0.000000 -0.000000");
      SpawnObject("Land_Cupboard_Door_1", "17462.099609 152.908005 3592.300049", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_Cupboard_Door_1", "17462.099609 152.908997 3592.560059", "-89.366951 0.000000 0.000000");
      SpawnObject("Land_Cupboard_Door_1", "17462.099609 152.839996 3590.010010", "-88.903564 0.000000 0.000000");
      SpawnObject("Land_Cupboard_Door_1", "17462.099609 152.834000 3589.590088", "-90.895279 0.000000 -0.000000");
      SpawnObject("Land_Cupboard_Door_1", "17462.099609 152.863007 3591.110107", "-89.366951 0.000000 0.000000");
      SpawnObject("Land_Cupboard_Door_1", "17462.000000 152.914993 3592.659912", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_Cupboard_Door_1", "17462.000000 152.921997 3591.790039", "-91.415512 0.000000 -0.000000");
      SpawnObject("Land_Cupboard_Door_1", "17462.000000 152.893005 3589.520020", "-91.415512 0.000000 -0.000000");
      SpawnObject("Land_Cupboard_Door_1", "17462.000000 152.895004 3590.040039", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_Cupboard_Door_1", "17462.000000 152.873993 3591.080078", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_Cupboard_Door_1", "17462.000000 152.876999 3590.459961", "-90.633041 0.000000 -0.000000");
      SpawnObject("Land_groggs_signage2", "17461.699219 154.475998 3602.719971", "179.674774 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_up", "17474.500000 152.253006 3595.070068", "-89.366951 0.000000 0.000000");
      SpawnObject("ANT_wooden_planks_up", "17474.699219 152.246002 3595.080078", "90.000000 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "17474.900391 155.595993 3598.419922", "-179.765686 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "17497.699219 155.589005 3598.419922", "179.951035 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "17474.500000 154.031998 3595.040039", "89.884087 0.000000 -0.000000");
      //// -- Grggs_Locksley.txt (9 objects)
      SpawnObject("Land_groggs_signage", "16184.200195 170.279999 5183.310059", "152.745300 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_up", "16236.799805 169.125000 5173.330078", "91.550667 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_up", "16236.900391 169.108994 5173.350098", "90.000000 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "16236.900391 170.860992 5173.399902", "-90.000000 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "16236.400391 172.412003 5169.939941", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "16213.599609 172.429993 5170.029785", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_up", "16091.099609 168.929993 5185.479980", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_poots_signage", "16091.200195 170.854004 5185.470215", "-179.206131 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "16212.599609 172.369995 5184.000000", "89.977844 0.000000 -0.000000");
      //// -- greased_latina.txt (5 objects)
      SpawnObject("ANT_wooden_planks_down", "17599.699219 154.669998 4426.069824", "90.000000 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_down", "17599.599609 154.660004 4429.080078", "-90.000000 0.000000 -0.000000");
      SpawnObject("ANT_wooden_planks_down", "17599.699219 154.654007 4424.870117", "-88.325081 0.000000 0.000000");
      SpawnObject("Land_greasedandreleased_signage", "17599.099609 155.591995 4421.759766", "-91.096420 0.000000 -0.000000");
      SpawnObject("Land_greasedandreleased_signage", "17599.400391 155.395996 4432.479980", "-88.584480 0.000000 0.000000");
      //// -- crookes.txt (1 objects)
      SpawnObject("Land_argod_signage", "17675.500000 153.718002 4194.350098", "89.883682 0.000000 -0.000000");
      //// -- argod_latina.txt (1 objects)
      SpawnObject("Land_argod_signage", "17675.500000 153.718002 4194.350098", "89.883682 0.000000 -0.000000");
      //// -- Angelofthenorth.txt (1 objects)
      SpawnObject("Land_AngelOTN", "6151.209961 211.860001 6304.970215", "-61.831993 0.000000 -0.000000");
      //// -- ZOOTOPIA.txt (1 objects)
      SpawnObject("Land_groggs_signage2", "9869.580078 164.089996 4285.640137", "0.000000 0.000000 -0.000000");
      //// -- WELL FARM MON.txt (1 objects)
      SpawnObject("Land_Misc_Well_Pump_Blue", "13661.299805 149.358002 6887.000000", "115.029015 0.000000 -0.000000");
      //// -- WAF Bunker climb over.txt (9 objects)
      SpawnObject("ANT_roadblock_woodencrate", "2652.739990 246.166000 6823.020020", "-88.449318 0.000000 0.000000");
      SpawnObject("ANT_roadblock_woodencrate", "2652.750000 246.811996 6823.049805", "-89.366951 0.000000 0.000000");
      SpawnObject("ANT_roadblock_woodencrate", "2653.850098 246.192001 6823.270020", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_misc_woodencrate_5x", "2651.830078 246.863998 6821.680176", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_misc_woodencrate_3x", "2653.580078 246.639008 6821.479980", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_misc_woodencrate", "2652.850098 246.218002 6820.339844", "-91.096420 0.000000 -0.000000");
      SpawnObject("ANT_misc_woodencrate", "2652.770020 247.440002 6823.089844", "-87.098770 0.000000 0.000000");
      SpawnObject("ANT_misc_woodencrate", "2653.870117 246.826004 6823.299805", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_misc_woodencrate", "2655.189941 246.201996 6823.310059", "-79.751152 0.000000 -0.000000");
      //// -- vladton well.txt (1 objects)
      SpawnObject("Land_Misc_Well_Pump_Yellow", "17652.400391 31.784901 8452.940430", "0.000000 0.000000 -0.000000");
      //// -- tosco_latina.txt (9 objects)
      SpawnObject("ANT_wooden_planks_up", "17113.400391 152.257004 4517.729980", "-89.366951 0.000000 0.000000");
      SpawnObject("ANT_wooden_planks_up", "17113.500000 152.279007 4517.720215", "-89.366951 0.000000 0.000000");
      SpawnObject("Land_tosco_signage", "17113.400391 154.011002 4517.709961", "90.000000 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "17113.800781 155.606995 4521.129883", "179.954529 0.000000 -0.000000");
      SpawnObject("Land_tosco_signage", "17136.699219 155.548004 4520.939941", "-179.769196 0.000000 -0.000000");
      SpawnObject("Land_groggs_signage2", "17144.900391 153.455002 4527.279785", "179.667755 0.000000 -0.000000");
      SpawnObject("Land_greasedandreleased_signage", "17116.199219 156.358002 4558.830078", "89.366951 0.000000 -0.000000");
      SpawnObject("Land_greasedandreleased_signage", "17116.099609 156.410995 4582.450195", "89.366951 0.000000 -0.000000");
      SpawnObject("Land_greasedandreleased_signage", "17117.400391 156.294998 4557.709961", "0.000000 0.000000 -0.000000");
      //// -- south_island_overpass.txt (1 objects)
      SpawnObject("StaticObj_Bridge_Wood_50_1", "4092.320068 3.635680 1396.410034", "-28.991842 0.000000 -0.000000");
      //// -- sneakbridge.txt (8 objects)
      SpawnObject("StaticObj_Bridge_Wood_50", "13911.099609 148.770004 7005.569824", "1.969231 0.000000 -0.000000");
      SpawnObject("StaticObj_Bridge_Wood_50", "13875.099609 148.479996 7006.729980", "1.594864 0.000000 -0.000000");
      SpawnObject("StaticObj_Bridge_Wood_50", "13841.299805 148.505005 7007.049805", "-0.657461 0.000000 0.000000");
      SpawnObject("StaticObj_Bridge_Wood_50_1", "10964.900391 143.901993 6768.169922", "75.908859 0.000000 -0.000000");
      SpawnObject("StaticObj_Bridge_Wood_50", "10973.599609 145.231995 6733.830078", "75.624390 0.000000 -0.000000");
      SpawnObject("StaticObj_Bridge_Wood_50", "10982.299805 145.199997 6699.450195", "75.908859 0.000000 -0.000000");
      SpawnObject("StaticObj_Bridge_Wood_25", "10986.500000 145.212006 6682.430176", "76.096817 0.000000 -0.000000");
      SpawnObject("StaticObj_Bridge_Wood_25", "13938.200195 148.955002 7004.439941", "1.125899 0.000000 -0.000000");
      //// -- shatwest_tower.txt (6 objects)
      SpawnObject("StaticObj_Furniture_SofaCorner", "17557.500000 181.584000 4166.580078", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Furniture_sofa_leather_new", "17558.199219 181.554993 4169.020020", "-91.479050 0.000000 -0.000000");
      SpawnObject("Chalkboard_1", "17554.199219 181.750000 4166.259766", "-86.412239 0.000000 -0.000000");
      SpawnObject("StaticObj_Furniture_lobby_chair", "17543.099609 181.166000 4168.569824", "-66.333984 0.000000 -0.000000");
      SpawnObject("StaticObj_Furniture_lobby_chair", "17543.099609 181.158997 4166.839844", "-100.298737 0.000000 -0.000000");
      SpawnObject("ANT_church_chair", "17548.500000 181.787003 4162.259766", "-179.862442 0.000000 -0.000000");
      //// -- Angelofthenorth.txt (1 objects)
      SpawnObject("Land_AngelOTN", "6151.209961 211.860001 6304.970215", "-61.831993 0.000000 -0.000000");
      //// -- westminster.txt (28 objects)
      SpawnObject("Land_westminster", "16917.000000 155.492996 4921.000000", "-1.000361 1.000209 0.040328");
      SpawnObject("Land_Westminsterdoor", "16908.699219 150.041000 4886.660156", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_Westminsterdoor", "16904.800781 150.076004 4890.370117", "89.366951 0.000000 -0.000000");
      SpawnObject("Land_rubbish_floor_1", "16904.199219 150.160995 4888.229980", "0.000000 0.000000 -0.000000");
      SpawnObject("Land_rubbish_floor_1", "16903.500000 150.130997 4888.919922", "-2.627210 0.000000 0.000000");
      SpawnObject("StaticObj_Garbage_Pile1", "16902.099609 150.548004 4888.419922", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Garbage_Pile2", "16902.599609 150.186996 4891.810059", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Garbage_Pile3", "16899.699219 150.210007 4889.740234", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Garbage_Pile4", "16903.000000 150.334000 4902.470215", "74.775604 0.000000 -0.000000");
      SpawnObject("StaticObj_Garbage_Pile5", "16913.000000 150.395004 4884.140137", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Garbage_Pile6", "16905.599609 150.315002 4884.660156", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_DirtPile_Medium2", "16903.300781 150.313995 4893.680176", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Glass", "16901.300781 150.143997 4897.689941", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Glass", "16903.599609 150.078003 4886.770020", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Glass", "16900.599609 150.091995 4892.700195", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Glass", "16903.000000 150.091995 4900.830078", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Wood1", "16900.900391 150.283005 4903.379883", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_DirtPile_Medium2", "16917.400391 150.470993 4883.910156", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_DirtPile_Medium1", "16918.400391 150.654999 4886.129883", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_DirtPile_Large", "16920.300781 150.912994 4886.640137", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_DirtPile_Large", "16896.300781 150.695007 4907.439941", "79.000366 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Concrete1", "16897.699219 152.464005 4936.569824", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Concrete2", "16910.599609 150.651001 4939.189941", "0.000000 0.000000 -0.000000");
      SpawnObject("StaticObj_Rubble_Bricks2", "16911.099609 152.216003 4946.479980", "-74.123383 0.000000 0.000000");
      SpawnObject("StaticObj_Rubble_DirtPile_Large", "16920.099609 151.927994 4954.890137", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_decal_heli_army", "16891.699219 152.091995 4970.939941", "0.000000 0.000000 -0.000000");
      SpawnObject("ANT_garbage_pile5", "16911.800781 151.703003 4953.649902", "0.000000 0.000000 -0.000000");
      SpawnObject("car3", "16911.000000 151.884003 4957.450195", "-14.918700 4.000000 -0.000000");
      //// -- tardis.txt (1 objects)
      SpawnObject("Land_tardis", "17544.099609 151.856995 4036.370117", "-90.165756 0.000000 -0.000000");
      //// -- stonehenge.txt (2 objects)
      SpawnObject("Land_stonehenge", "15442.200195 207.182007 6974.970215", "50.001686 2.998346 -0.037195");
      SpawnObject("Land_stonehengesign", "15455.500000 207.350998 6988.089844", "-79.561623 0.000000 -0.000000");
      //// -- Mansion.txt (30 objects)
      SpawnObject( "OU_dk", "4353.279785 213.961996 4582.430176", "-83.119339 0.000000 0.000000" );
      SpawnObject( "StaticObj_Misc_Fountain", "4355.339844 208.707993 4595.879883", "7.505145 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Statue_Enoch1", "4369.629883 209.531006 4590.629883", "109.773041 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Statue_Enoch1", "4341.319824 209.511002 4595.180176", "-82.189369 0.000000 0.000000" );
      SpawnObject( "Land_Misc_Greenhouse", "4317.640137 209.457993 4585.500000", "-174.508194 0.000000 -0.000000" );
      SpawnObject( "Land_Misc_Greenhouse", "4317.200195 209.464005 4581.430176", "-173.576431 0.000000 -0.000000" );
      SpawnObject( "Land_Shed_W6", "4318.830078 209.513000 4599.549805", "-168.133759 0.000000 -0.000000" );
      SpawnObject( "PSD_cobblestone", "4353.709961 207.893005 4582.770020", "8.999920 0.000000 -0.000000" );
      SpawnObject( "ANT_dam_stone_40", "4350.729980 206.270996 4564.240234", "-169.420914 -89.104088 -1.194936" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Large", "4329.740234 207.733002 4570.620117", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Large", "4329.919922 207.380997 4567.310059", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Large", "4329.399902 207.589996 4565.580078", "0.000000 8.999920 -2.998346" );
      SpawnObject( "StaticObj_Rubble_DirtPile_Large", "4329.569824 207.722000 4569.169922", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_garbage_pile7", "4325.770020 207.664001 4570.819824", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_garbage_pile7", "4324.970215 207.432999 4569.109863", "-161.473129 0.000000 -0.000000" );
      SpawnObject( "ANT_garbage_pile8", "4325.379883 207.919006 4569.930176", "0.000000 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4338.669922 209.042007 4606.359863", "99.078606 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4336.060059 209.029007 4604.500000", "9.288628 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4335.560059 209.056000 4601.450195", "9.288580 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4343.259766 209.029007 4605.629883", "-81.064529 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4347.729980 209.029007 4604.919922", "-80.970901 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4375.229980 209.029007 4600.609863", "-81.533394 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4370.830078 209.029007 4601.279785", "-81.346138 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4366.350098 209.029007 4601.970215", "-80.876488 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4362.290039 209.029007 4602.620117", "-80.970833 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4351.830078 209.029007 4604.259766", "-81.345879 0.000000 -0.000000" );
      SpawnObject( "Land_Wall_Gate_Fen3", "4357.069824 209.177994 4603.390137", "8.913297 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4377.200195 209.029007 4598.060059", "8.256555 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4376.569824 209.029007 4593.600098", "7.787429 0.000000 -0.000000" );
      SpawnObject( "hmansion", "4375.950195 209.029007 4589.040039", "8.068853 0.000000 -0.000000" );
      SpawnObject( "ANT_t_salixalba_2s", "4306.220215 213.690002 4600.169922", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_salixalba_2s", "4388.149902 213.835999 4598.299805", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_salixalba_2s", "4379.709961 212.134995 4544.830078", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_salixalba_2sb_swamp", "4359.709961 211.147003 4547.270020", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_salixalba_2sb_summer", "4344.180176 207.858994 4552.129883", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_t_salixalba_2s", "4320.459961 210.136002 4557.279785", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_prunusspinosa_2s_summer", "4328.649902 210.487000 4598.959961", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_b_prunusspinosa_1s_summer", "4330.850098 209.598999 4596.299805", "0.000000 0.000000 -0.000000" );
      //// ## Devils Arse Optional charger for second endgame (in bunker config below) and milli bits
      SpawnObject( "StaticObj_Power_TransformerStation_Big", "5388.069824 600.882996 9250.910156", "-20.453802 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big2_5", "5366.560059 600.163025 9240.940430", "69.050980 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big2_1", "5377.959961 600.158020 9233.309570", "-20.265724 0.000000 0.000000" );
      SpawnObject( "Land_Medical_Tent_Big", "5404.089844 598.166016 9224.580078", "-22.423368 0.000000 0.000000" );
      SpawnObject( "Land_Medical_Tent_Big", "5401.029785 598.130981 9232.259766", "-22.142517 0.000000 0.000000" );
      SpawnObject( "Land_Mil_Tent_Big1_1", "5386.770020 598.125000 9219.990234", "-164.381775 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big2_3", "5345.959961 600.168030 9224.570313", "-34.904099 0.000000 0.000000" );
      SpawnObject( "Land_Mil_Tent_Big3", "5381.970215 599.192993 9186.000000", "-21.768023 0.000000 0.000000" );
      SpawnObject( "Land_Mil_Tent_Big2_5", "5365.270020 600.161011 9178.639648", "-21.485882 0.000000 0.000000" );
      SpawnObject( "Land_Container_1Mo", "5354.279785 599.270996 9183.910156", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "5348.990234 599.273987 9181.389648", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "5360.270020 599.323975 9187.809570", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "5360.040039 599.252991 9166.639648", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "5352.189941 599.255005 9167.690430", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big4", "5373.919922 599.343994 9276.940430", "-62.486481 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_CamoNet_Tent2", "5362.450195 600.083984 9266.830078", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_MobileLaboratory", "5359.330078 598.908020 9254.679688", "-91.415512 0.000000 -0.000000" );
      SpawnObject( "Land_MobileLaboratory", "5323.040039 598.854004 9222.559570", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big1_5", "5358.950195 598.101990 9297.940430", "-21.298092 0.000000 0.000000" );
      SpawnObject( "Land_Mil_Tent_Big1_5", "5350.339844 598.181030 9294.389648", "-20.829659 0.000000 0.000000" );
      SpawnObject( "Land_Mil_Tent_Big1_3", "5342.270020 598.132019 9290.809570", "-22.985794 0.000000 0.000000" );
      SpawnObject( "StaticObj_Mil_CamoNet_Side_east", "5332.629883 600.872009 9286.679688", "-24.111162 0.000000 0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "5335.060059 598.426025 9286.929688", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "5329.830078 598.432007 9285.459961", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "5333.069824 599.085022 9288.950195", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "5336.029785 599.085022 9289.519531", "91.415512 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "5360.950195 599.054016 9271.179688", "59.294384 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "5360.350098 599.057007 9265.830078", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_3x", "5358.859863 598.789001 9269.209961", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "5361.729980 598.390991 9268.169922", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "5358.770020 598.426025 9271.480469", "63.897739 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "5361.359863 598.393982 9242.299805", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Roadblock_WoodenCrate", "5363.259766 598.395996 9236.660156", "-47.850300 0.000000 0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "5394.890137 598.994995 9217.950195", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "5389.089844 599.036987 9239.940430", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate_5x", "5392.100098 599.080994 9241.330078", "89.104713 0.000000 -0.000000" );
      SpawnObject( "Land_Castle_Donjon", "5353.040039 657.065979 9223.969727", "-22.894657 0.000000 0.000000" );
      SpawnObject( "Land_Castle_Donjon", "5365.830078 656.364014 9229.089844", "-22.141302 0.000000 0.000000" );
      SpawnObject( "Land_Castle_Donjon", "5360.350098 653.406006 9209.290039", "-22.894657 0.000000 0.000000" );
      SpawnObject( "Land_Castle_Donjon", "5351.790039 654.070007 9212.910156", "-18.014950 0.000000 0.000000" );
      SpawnObject( "Land_Castle_Donjon", "5366.290039 656.794983 9217.000000", "-20.736145 0.000000 0.000000" );
      SpawnObject( "ANT_castle_stairs", "5341.669922 646.357971 9199.440430", "69.149002 0.000000 -0.000000" );
      SpawnObject( "Land_Castle_Donjon", "5300.459961 656.687988 9238.889648", "-22.048752 0.000000 -0.000000" );
      SpawnObject( "Land_Castle_Donjon", "5342.660156 657.052979 9253.179688", "-22.236439 0.000000 -0.000000" );
      SpawnObject( "Land_Castle_Donjon", "5328.000000 655.223999 9189.259766", "-22.048824 0.000000 -0.000000" );
      SpawnObject( "ANT_castle_stairs", "5354.759766 635.460999 9290.490234", "-116.054787 0.000000 -0.000000" );
      SpawnObject( "Land_Mil_Tent_Big2_2", "5318.810059 600.065002 9252.620117", "-89.132950 0.000000 -0.000000" );
      //// ## Sector 12
      SpawnObject( "Land_Cave_rock1", "16313.200195 6.567890 16976.199219", "94.762924 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16314.200195 6.197460 16971.800781", "40.062546 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16324.200195 6.119180 16967.199219", "94.762924 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16326.200195 5.552820 16959.900391", "94.762924 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16308.700195 6.628230 16971.800781", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16304.099609 6.511570 16971.400391", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16300.900391 6.503830 16969.699219", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16296.799805 6.252860 16967.900391", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16292.099609 5.613590 16966.900391", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16287.000000 6.086680 16968.199219", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16279.599609 6.319130 16967.099609", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16274.799805 6.764260 16965.900391", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16273.700195 6.890220 16965.300781", "-58.921944 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16264.799805 8.867050 16956.500000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16261.400391 6.386400 16961.800781", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16264.799805 7.095250 16964.099609", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16262.200195 3.972660 16957.599609", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16326.900391 1.154940 16959.400391", "94.762924 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16261.700195 0.564513 16955.099609", "121.409027 0.000000 -0.000000" );
      SpawnObject( "Land_Cave_rock1", "16264.500000 4.039150 16954.599609", "23.456175 0.000000 -0.000000" );
      
      //// ## Ship
      SpawnObject( "Land_Ship_Big_FrontA", "9452.860352 17.975201 10516.500000", "-0.279766 0.000000 0.000000" );
      SpawnObject( "Land_Ship_Big_FrontB", "9433.049805 16.416300 10514.200195", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Ship_Big_BackA", "9394.040039 11.637800 10524.400391", "23.000095 0.000000 0.000000" );
      SpawnObject( "Land_Ship_Big_BackB", "9376.240234 10.160900 10532.599609", "23.268837 0.000000 -0.000000" );
      SpawnObject( "Land_Ship_Big_Castle", "9386.570313 20.419300 10528.099609", "22.893044 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_Flagpole", "9413.709961 1.829240 10515.500000", "0.000000 0.000000 -68.000000" );
      SpawnObject( "Land_Container_1Mo", "9429.889648 2.075300 10506.799805", "90.353340 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "9354.290039 3.438110 10541.500000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "9362.830078 3.424700 10542.599609", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "9464.599609 8.667980 10515.000000", "-36.591522 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "9444.139648 1.956480 10515.000000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "Land_Container_1Mo", "9449.419922 1.974190 10513.500000", "0.000000 0.000000 -0.000000" );      
      //// ## Minor fixes
      SpawnObject( "StaticObj_Misc_WoodenCrate", "16431.500000 151.261002 3359.139893", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_Misc_WoodenCrate", "16431.500000 151.656998 3359.139893", "0.000000 0.000000 -0.000000" );
      SpawnObject( "StaticObj_WaterSpring_Sakhal", "7474.709961 151.300003 7270.209961", "118.496620 0.000000 -0.000000" );
      SpawnObject( "Land_Misc_FireBarrel_Green", "7437.379883 151.714005 7250.890137", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_monolith1", "9564.849609 48.777401 15813.000000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_monolith1", "9568.980469 42.457298 15792.500000", "-32.557579 0.000000 0.000000" );
      SpawnObject( "ANT_rock_monolith1", "9571.410156 44.448601 15809.599609", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_monolith1", "9554.049805 33.676601 15751.500000", "34.432133 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_monolith1", "9573.120117 33.058102 15759.400391", "95.873093 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_monolith1", "9586.500000 36.082802 15750.000000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_monolith1", "9585.389648 34.752800 15729.000000", "48.132469 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_monolith1", "9572.700195 35.260700 15723.500000", "71.397392 0.000000 -0.000000" );
      SpawnObject( "ANT_rock_monolith1", "9555.870117 33.834999 15730.500000", "-37.906872 0.000000 0.000000" );
      SpawnObject( "Land_Underground_Panel_Lever", "9591.230469 43.427700 15795.299805", "-47.944187 0.000000 0.000000" );
      SpawnObject( "ANT_misc_boxwooden_prx", "9589.980469 41.980801 15795.099609", "-44.095932 0.000000 0.000000" );
      SpawnObject( "ANT_misc_boxwooden_prx", "9589.269531 41.981300 15795.900391", "-44.095932 0.000000 0.000000" );
      SpawnObject( "ANT_misc_boxwooden_prx", "9590.000000 43.000000 15795.000000", "0.000000 0.000000 -0.000000" );
      SpawnObject( "ANT_misc_boxwooden_prx", "9589.290039 42.954700 15795.799805", "0.000000 0.000000 -0.000000" );


        //INIT ECONOMY--------------------------------------
        Hive ce = CreateHive();
        if (ce)
                ce.InitOffline();

        //DATE RESET AFTER ECONOMY INIT-------------------------
        int year, month, day, hour, minute;
        int reset_month = 2, reset_day = 20;

        GetGame().GetWorld().GetDate(year, month, day, hour, minute);

        if ((month == reset_month) && (day < reset_day)) {
                GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
        } else {
                if ((month == reset_month + 1) && (day > reset_day)) {
                        GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
                } else {
                        if ((month < reset_month) || (month > reset_month + 1)) {
                                GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
                        }
                }
        }
        //corrected export data for mapsize, ElektroVodka
        //CEApi TestHive = GetCEApi();
        //TestHive.ExportProxyProto();
        //TestHive.ExportProxyData( "10240 0 10240", 26384 );
        //TestHive.ExportClusterData();      
        if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
                // CLEAN-UP LAST BUNKER SETUP
                CleanupSciFiWalls();

                // SPAWN LANDINGPAD
                SpawnDIBunkerObject("fmdpk_landingpad_1", "19726.5 71.16 933.31", "-125 0 0", 1);

                // GENERATE RANDOM BUNKER LAYOUT
                // GenerateMap("6000 3500 6000");
                GenerateMap("19547.75 25.25 782.76");

                // SPAWN RECHARGE NODES
                SpawnDIBunkerObject("fmdpk_landingpad_cardterminal", "19571.7 80.78 651.671", "115.0 0 0", 1);
		SpawnDIBunkerObject("fmdpk_landingpad_cardterminal", "5379.09 598.18 9258.410", "70.74 0 0", 1);

                // SPAWN GAS ZONE
                // vector gasOrigin = "6000 3500 6000";
                // SpawnDIBunkerObject("ContaminatedArea_Static", gasOrigin, "0 0 0", 1.0);

                // ZOMBIE SPAWNING, TRIGGER ZONE AT THE BUNKER ENTRANCE
                GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(FP_StartBunkerZone, 1000, false);
        }
}

class CustomMission: MissionServer {
        void SetRandomHealth(EntityAI itemEnt) {
                if (itemEnt) {
                        int rndHlt = Math.RandomInt(55, 100);
                        itemEnt.SetHealth("", "", rndHlt);
                }
        }

        override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName) {
                Entity playerEnt;
                string characterType;

                if (GetGame().ListAvailableCharacters().Find(GetGame().GetMenuDefaultCharacterData().GetCharacterType()) > -1)
                        characterType = GetGame().GetMenuDefaultCharacterData().GetCharacterType();
                else
                        characterType = GetGame().CreateRandomPlayer();

                playerEnt = GetGame().CreatePlayer(identity, characterType, pos, 0, "NONE");

                Class.CastTo(m_player, playerEnt);

                GetGame().SelectPlayer(identity, m_player);

                return m_player;
        }

        override void StartingEquipSetup(PlayerBase player, bool clothesChosen) {
                // Clean slate first
                player.RemoveAllItems();

                EntityAI itemEnt;
                ItemBase itemBs;

                // --- Your four freshy variants (unchanged) ---
                switch (Math.RandomInt(0, 4)) // 0..3 inclusive
                {
                case 0:
                        // Freshy 0
                        player.GetInventory().CreateInInventory("CostCutter");
                        player.GetInventory().CreateInInventory("JumpsuitPants_Grey");
                        player.GetInventory().CreateInInventory("HikingBoots_Black");
                        player.GetInventory().CreateInInventory("Apple");
                        player.GetInventory().CreateInInventory("Plum");
                        player.GetInventory().CreateInInventory("BoneKnife");
                        break;

                case 1:
                        // Freshy 1
                        player.GetInventory().CreateInInventory("MarmiteShirt");
                        player.GetInventory().CreateInInventory("JumpsuitPants_Grey");
                        player.GetInventory().CreateInInventory("HikingBoots_Black");
                        player.GetInventory().CreateInInventory("Apple");
                        player.GetInventory().CreateInInventory("Plum");
                        player.GetInventory().CreateInInventory("BoneKnife");
                        break;

                case 2:
                        // Freshy 2
                        player.GetInventory().CreateInInventory("TescoShirt");
                        player.GetInventory().CreateInInventory("JumpsuitPants_Grey");
                        player.GetInventory().CreateInInventory("HikingBoots_Black");
                        player.GetInventory().CreateInInventory("Apple");
                        player.GetInventory().CreateInInventory("Plum");
                        player.GetInventory().CreateInInventory("BoneKnife");
                        break;

                case 3:
                        // Freshy 3
                        player.GetInventory().CreateInInventory("NettoShirt");
                        player.GetInventory().CreateInInventory("JumpsuitPants_Grey");
                        player.GetInventory().CreateInInventory("HikingBoots_Black");
                        player.GetInventory().CreateInInventory("Apple");
                        player.GetInventory().CreateInInventory("Plum");
                        player.GetInventory().CreateInInventory("BoneKnife");
                        break;
                }

                // --- Universal gear ---
                itemEnt = player.GetInventory().CreateInInventory("BandageDressing");
                itemBs = ItemBase.Cast(itemEnt);
                if (itemBs)
                        itemBs.SetQuantity(4);
        }
};

Mission CreateCustomMission(string path) {
        return new CustomMission();
}