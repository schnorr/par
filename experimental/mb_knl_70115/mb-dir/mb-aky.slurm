#!/bin/bash
#SBATCH --time=02:00:00
#SBATCH --output=%j.out
#SBATCH --error=%j.err
#SBATCH --partition=knl --nodes=4 --ntasks=272

echo "I am the script running at $(hostname)"

# draco (16 cores per node) 4 is 64, 7 is 112
# knl (68 cores per node) = 272 cores (4 nodes)

# Função para gerar um arquivo de máquina para uma partição
function gen_machinefile {
	SLM_NODES=$1
	SLM_TASKS=$2

	if [ -z "$SLM_NODES" ]; then
		return
	fi

	for host in $(scontrol show hostname $SLM_NODES); do
		for machine in $(seq 1 $SLM_TASKS); do
			echo $host
		done
	done
}


DIR=$HOME/mb-dir/

# Create a directory
EXPEDIR=mb_${SLURM_JOB_PARTITION}_${SLURM_JOB_ID}
mkdir -p $EXPEDIR
pushd $EXPEDIR

# Prepare the machine file (for multi partition)
MACHINEFILE="nodes.$SLURM_JOB_ID"
gen_machinefile $SLURM_JOB_NODELIST_PACK_GROUP_0 $SLURM_NPROCS_PACK_GROUP_0 > $MACHINEFILE
gen_machinefile $SLURM_JOB_NODELIST_PACK_GROUP_1 $SLURM_NPROCS_PACK_GROUP_1 >> $MACHINEFILE
gen_machinefile $SLURM_JOB_NODELIST_PACK_GROUP_2 $SLURM_NPROCS_PACK_GROUP_2 >> $MACHINEFILE

# Prepare the machine file
srun -l /bin/hostname | sort -n | awk '{print $2}' > $MACHINEFILE

for m in $(cat $MACHINEFILE | uniq); do
    ssh $m hostname
done

# Get number of cores available
NP=$(cat $MACHINEFILE | wc -l)

# The mandelbrot-mpi binary location
BINARY=${DIR}/mandelbrot-mpi-akypuera

echo "Compile the application at $DIR"
rm -f $BINARY
make -C $DIR

# Copy source files to experimental directory
cp -prfv $DIR .

# Application parameters
XMIN=0.27085
XMAX=0.27100
YMIN=0.004640
YMAX=0.004810
MAXITER=64000

echo "Define the resolution based on the number of NP == $NP"
function def_resolution {
    NP=$1
    ELEMENTS=$2
    if [[ $NP = 0 ]]; then
	echo "Error: $NP == 0"
	return -1
    fi
    SIDE=$(echo "sqrt($NP*$ELEMENTS)" | bc -l)
    FLOOR=$(echo ${SIDE%.*})
    let i=$FLOOR
    let x=($i*$i)%$NP
    while [[ $x != 0 ]]; do
	let i=$i+1
	let x=($i*$i)%$NP    
    done
    echo $i
}
XRES=$(def_resolution $NP 6000000)
echo "Resolution is $XRES"

# Akypuera
export LD_LIBRARY_PATH=/scratch/$USER/install/akypuera/lib/
export LD_LIBRARY_PATH=$HOME/install/akypuera/lib/

echo "Let's launch the $BINARY application..."

# Execute the program
COMMAND="mpirun \
	--mca oob_tcp_if_include 192.168.30.0/24 \
	--mca btl_tcp_if_include 192.168.30.0/24 \
	--mca btl_base_warn_component_unused 0 \
	-np $NP \
	-machinefile $MACHINEFILE \
	$BINARY $XMIN $XMAX $YMIN $YMAX $MAXITER $XRES"
echo "Let's execute '$COMMAND'"
eval $COMMAND

echo "mpirun return code is $?".

echo "Post-processing trace files..."

# Remove previously generated files
rm -f rastro.paje rastro.csv

# Convert trace files to a CSV
$HOME/install/akypuera/bin/aky_converter rastro*.rst > rastro.paje
$HOME/install/pajeng/bin/pj_dump rastro.paje | grep ^State > rastro.csv

# Remove rastro files
rm -f rastro*.rst

echo "The SLURM script (with akypuera) has finished."

popd

# Move stdout and stderr log files
mv $SLURM_JOB_ID.out $SLURM_JOB_ID.err $EXPEDIR
