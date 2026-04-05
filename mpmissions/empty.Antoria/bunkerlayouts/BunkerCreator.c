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