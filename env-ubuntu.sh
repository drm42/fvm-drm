export LD_LIBRARY_PATH=/home/drmoser/fvm-drm/build-ubuntu/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/drmoser/fvm-drm/build-ubuntu/lib:/home/drmoser/fvm-drm/build-ubuntu/lib64/python2.7/site-packages:/home/drmoser/fvm-drm/build-ubuntu/lib/python2.7/site-packages:/home/drmoser/fvm-drm/build-ubuntu/bin:/home/drmoser/fvm-drm/build-ubuntu/lib:/home/drmoser/fvm-drm/build-ubuntu/lib64/python2.7/site-packages:/home/drmoser/fvm-drm/build-ubuntu/lib/python2.7/site-packages:/home/drmoser/fvm-drm/build-ubuntu/bin
# Add include and lib directories to compiler paths
export C_INCLUDE_PATH=/home/drmoser/fvm-drm/build-ubuntu/include:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=/home/drmoser/fvm-drm/build-ubuntu/include:$CPLUS_INCLUDE_PATH
export PATH=/home/drmoser/fvm-drm/build-ubuntu/bin:$PATH

# Need this to recompile MPM in its directory.
export MEMOSA_CONFNAME=ubuntu
