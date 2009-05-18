#!/usr/bin/env python
import pdb
import sys
sys.setdlopenflags(0x100|0x2)
from mpi4py import MPI
import fvmbaseExt
import importers
#pdb.set_trace()

atype = 'double'
if atype == 'double':
    import models_atyped_double as models
    import exporters_atyped_double as exporters
elif atype == 'tangent':
    import models_atyped_tangent_double as models
    import exporters_atyped_tangent_double as exporters


from FluentCase import FluentCase
from fvmbaseExt import vectorInt
from fvmbaseExt import VecD3

reader = FluentCase("cav32.cas")
reader.read();
meshes = reader.getMeshList()

geomFields =  models.GeomFields('geom')
metricsCalculator = models.MeshMetricsCalculatorA(geomFields,meshes)
metricsCalculator.init()
flowFields =  models.FlowFields('flow')
fmodel = models.FlowModelA(geomFields,flowFields,meshes)
reader.importFlowBCs(fmodel)
octree = fvmbaseExt.Octree()
mesh0 = meshes[0]
cells = mesh0.getCells()
nCells = cells.getCount()
cellCentroid = geomFields.coordinate[cells].asNumPyArray()

"""
cellCoord = open(fileBase+"cellCentroid.dat", "w")
for c in range(0, nCells):
    cellCoord.write("%lf\t%lf\t%lf\n" % (cellCentroid[c][0], cellCentroid[c][1], cellCentroid[c][2]))
cellCoord.close()
"""

octree.Impl(mesh0, geomFields)

### search the closest point ###
Points = [(0,   0,   0,  1088),
          (1,   1,   0,  1087),
          (0,   1,   0,  1151),
          (1,   0,   0,  1119),
          (0.5, 0.5, 0,  495),
          (-1,  -1,  0,  1088),
          (2,   2,   0,  1087),
          (0,   2,   0,  1024),
          (2,   0,   0,  1056),
          (0.02,0.02,0,  1023),
          ]

def test_search_singlePoint():
    for point in Points:
        yield search_SP, point

#@with_setup(setup_func, teardown_func)
def search_SP(point):
    x, y, z, result = point
    node = octree.getNode(x, y, z)
    assert node == result

### search the points within a range ###
def test_search_within_Radius_1():
    testpoint = VecD3()
    testpoint[0] = 0.5
    testpoint[1] = 0.5
    testpoint[2] = 0.0
    radius = 0.03
    nodeList = vectorInt()
    octree.getNodes(testpoint, radius, nodeList)
    nNode = nodeList.size()
    assert nNode == 4
    assert nodeList[0] == 528
    assert nodeList[1] == 496
    assert nodeList[2] == 527
    assert nodeList[3] == 495


def test_search_within_Radius_2():
    testpoint = VecD3()
    testpoint[0] = 0.0
    testpoint[1] = 0.0
    testpoint[2] = 0.0
    radius = 0.03
    nodeList = vectorInt()
    octree.getNodes(testpoint, radius, nodeList)
    nNode = nodeList.size()
    assert nNode == 3
    assert nodeList[0] == 1088
    assert nodeList[1] == 1120
    assert nodeList[2] == 1023












