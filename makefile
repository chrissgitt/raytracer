#define link target
linkTarget = chrisRay

#define the libraries that we need
LIBS = -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2

#define any flags
CFLAGS = -std=gnu++2c -Ofast

objects = main.o \
		CApp.o \
		$(patsubst %.cpp,%.o,$(wildcard ./ChrisRayTracer/*.cpp)) \
		$(patsubst %.cpp,%.o,$(wildcard ./ChrisRayTracer/cgPrimitives/*.cpp)) \
		$(patsubst %.cpp,%.o,$(wildcard ./ChrisRayTracer/cgMaterials/*.cpp)) \
		$(patsubst %.cpp,%.o,$(wildcard ./ChrisRayTracer/cgLights/*.cpp)) \
		$(patsubst %.cpp,%.o,$(wildcard ./ChrisRayTracer/cgTextures/*.cpp)) \
		$(patsubst %.cpp,%.o,$(wildcard ./ChrisRayTracer/cgRayMarch/*.cpp))

#define rebuildables
rebuildables = $(objects) $(linkTarget)

#rule to actually perform the build
$(linkTarget): $(objects)
	g++ -g -o $(linkTarget) $(objects) $(LIBS) $(CFLAGS)

#rule to create the .o or (object) files
%.o: %.cpp
	g++ -o $@ -c $< $(CFLAGS) $(LIBS)

.PHONEY:
clean:
	rm $(rebuildables)
