import unreal

pipeline = unreal.InterchangeGenericMeshPipeline()
print("Pipeline properties:")
for prop in dir(pipeline):
    if "skeletal" in prop.lower() or "mesh" in prop.lower():
        print("  ", prop)
