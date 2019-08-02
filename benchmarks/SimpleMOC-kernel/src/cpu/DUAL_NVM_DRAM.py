import sst
import os
import sys

if len(sys.argv) < 2:
	print ("Error, no executable specified")
	print ("Usage: sst DUAL_NVM_DRAM.py Executable")
	exit()
else:
	Executable = str(os.getcwd()) + "/" + str(sys.argv[1])

class RtrPorts:
	def __init__(self):
            self._next_addr = 0 
	def nextPort(self):
		res = self._next_addr
		self._next_addr = self._next_addr + 1
        	return res
	def numPorts(self):
		return self._next_addr


rtrInfo = RtrPorts()

memController = sst.Component("memController", "merlin.hr_router")
memController.addParams({
	"id" : 0,
	"topology" : "merlin.singlerouter",
	"link_bw" : "320GB/s",
	"xbar_bw" : "512GB/s",
	"input_latency" : "4ns",
	"output_latency" : "4ns",
	"input_buf_size" : "4KiB",
	"output_buf_size" : "4KiB",
	"flit_size" : "72B",
	})

pagesize = 4096
memoryperlevel=4096

corecount = 4;

clock = "2GHz"
cacheFrequency = clock
cacheLineSize = 64
memory_clock = "250MHz"

coherenceProtocol = "MESI"

ariel = sst.Component("a0", "ariel.ariel")
ariel.addParams({
	"verbose" : "3",
	"clock" : clock,
	"maxcorequeue" : 1024,
	"maxissuepercycle" : 3,
	"maxtranscore" : 16,
	"pipetimeout" : 0,
	"corecount" : corecount,
	"memmgr" : "ariel.MemoryManagerMalloc",
	"memmgr.memorylevels" : "2",
	"memmgr.pagecount0" : (memoryperlevel * 1024 * 1024) / pagesize,
	"memmgr.pagecount1" : (memoryperlevel * 1024 * 1024) /pagesize,
	"memmgr.defaultlevel" : 0,
	"arielmode" : "2",
	#"mallocmapfile" : "malloc.txt",
	#"opal_enabled" : 1,
	"arielinterceptcalls" : "1",
	"max_insts" : 10000000000,
	"executable" : str(Executable),
	"appargcount" : 4,
	"apparg0" : "-s",
	"apparg1" : "30000",
	"apparg2" : "-e",
	"apparg3" : "2",
	})

# Shared L3 -- added so L1 and L2 can connect to it
#l3 = sst.Component("L3cache", "memHierarchy.Cache")
#l3cache_net_addr = rtrInfo.nextPort()
#l3.addParams({
#        "cache_frequency" : cacheFrequency,
#        "cache_size"    : "8 MB",
#        "cache_line_size" :cacheLineSize,
#        "associativity":  "8",
#        "access_latency_cycles": "20",
#        "coherence_protocol" : coherenceProtocol,
#        "L1" : "0",
#       	"statistics" : 1,
#       	"mshr_num_entries" : 16, 
#       	"low_network_links" : 1,
#	"network_address" : l3cache_net_addr,
#        }) 

for x in range(corecount) :
	# The l1 cache is local
	l1 = sst.Component("l1cache_%d"%x, "memHierarchy.Cache")
       	l1.addParams({
		"cache_frequency"       : cacheFrequency,
		"cache_size"            : "32KB",
		"cache_line_size"       : cacheLineSize,
       		"associativity"         : "8",
	        "access_latency_cycles" : "4",
	        "coherence_protocol"    : coherenceProtocol,
	        "L1"                    : "1",
        	"statistics"            : "1",
	})

	# The l2 cache is local
	l2_net_addr = rtrInfo.nextPort()
	l2 = sst.Component("l2cache_%d"%x, "memHierarchy.Cache")
	l2.addParams({
		"cache_frequency"       : cacheFrequency,
		"cache_size"            : "256 KB",
	        "cache_line_size"       : cacheLineSize,
	        "associativity"         : "8",
	        "access_latency_cycles" : "10",
	        "coherence_protocol"    : coherenceProtocol,
	       	"L1"                    : "0",
              	"statistics"            : "1",
	      	"mshr_num_entries" : "16",
		"network_address" : l2_net_addr,
	})
	# Connect the Cache to the Core
	ArielL1Link = sst.Link("cpu_cache_%d"%x)
	ArielL1Link.connect((ariel, "cache_link_%d"%x, "60ps"), (l1, "high_network_0", "60ps"))
	L1L2Link = sst.Link("l1_l2%d"%x)
	L1L2Link.connect((l1, "low_network_0", "60ps"), (l2, "high_network_0", "60ps"))
	L2Link = sst.Link("l2_link_" + str(x))
	L2Link.connect((l2, "directory", "60ps"), (memController, "port" + str(l2_net_addr), "60ps"))
	#L2Link.connect((l2, "low_network_0", "60ps"), (l3, "high_network_0", "60ps"))

#Shared DRAM
DRAM = sst.Component("DRAM", "memHierarchy.MemController")
DRAM.addParams({
	"coherence_protocol" : coherenceProtocol,
	"rangeStart" : 0,
	"rangeEnd" : memoryperlevel * 1024 * 1024,
	"backing" : "none",
	"backend" : "memHierarchy.timingDRAM",
	"backend.id" : 0,
	"backend.addrMapper" : "memHierarchy.roundRobinAddrMapper",
	"backend.addrMapper.interleave_size" : "64B",
	"backend.addrMapper.row_size" : "1KiB",
	"backend.clock" : clock,
        "backend.mem_size" : str(memoryperlevel) + "MiB",
	"backend.channels" : 3,
	"backend.channel.numRanks" : 3,
	"backend.channel.rank.numBanks" : 5,
	"backend.channel.transaction_Q_size" : 32,
	"backend.channel.rank.bank.CL" : 14,
	"backend.channel.rank.bank.CL_WR" : 12,
	"backend.channel.rank.bank.RCD" : 14,
	"backend.channel.rank.bank.TRP" : 14,
	"backend.channel.rank.bank.dataCycles" : 2,
        "backend.channel.rank.bank.pagePolicy" : "memHierarchy.simplePagePolicy",
	"backend.channel.rank.bank.transactionQ" : "memHierarchy.reorderTransactionQ",
        "backend.channel.rank.bank.pagePolicy.close" : 0,
	"coherence_protocol" : coherenceProtocol,
	"debug" : 0,
	"debug_level" : 5
	})

DRAM_dc_port = rtrInfo.nextPort()
DRAM_dc = sst.Component("DRAM_dc", "memHierarchy.DirectoryController")
DRAM_dc.addParams({
	"network_bw" : "320GB/s",
	"coherence_protocol" : coherenceProtocol,
	"addr_range_start" : 0,
	"addr_range_end" : memoryperlevel * 1024 * 1024,
	"entry_cache_size" : 128 * 1024,
	"clock"  : clock,
	"statistics" : 1,
	"network_address" : DRAM_dc_port,
	})


# Shared NVM
slow_memory = sst.Component("slow_memory", "memHierarchy.MemController")
slow_memory.addParams({
        "coherence_protocol" : coherenceProtocol,
        "access_time" : "60ns",
        "backend.mem_size" : str(memoryperlevel) + "MiB",
        "rangeStart" : 0,
	"rangeEnd" : memoryperlevel * 1024 * 1024,
	"clock" : memory_clock,
        "use_dramsim" : "0",
	"backing" : "none",
	"backend" : "memHierarchy.Messier",
	"backendConvertor.backend" : "memHierarchy.Messier",
	"backend.clock" : memory_clock,
	"statistics" : 1
	})


slow_dc_port = rtrInfo.nextPort()
slow_dc = sst.Component("slow_dc", "memHierarchy.DirectoryController")
slow_dc.addParams({
        "coherence_protocol" : coherenceProtocol,
	"network_bw" : "320GB/s",#memoryperlevel * 1024 * 1024,
	"addr_range_start" : memoryperlevel * 1024 * 1024,
	"addr_range_end" : memoryperlevel * 1024 * 1024 * 2,
	"entry_cache_size" : 128 * 1024,
	"clock" : "1GHz",
	"statistics" : 1,
	"network_address" : slow_dc_port,
	})



NVM_controller = sst.Component("NVM_controller", "Messier")
NVM_controller.addParams({
	"clock" : memory_clock,
	"tCL" : 30, 
	"tRCD" : 300,
	"tCL_W" : 10, #1000
	"write_buffer_size" : 32, #32 
	"flush_th" : 90, 
	"num_banks" : 16, 
	"max_outstanding" : 16, 
	"max_writes" : "4",
	"max_current_weight" : 32*50,
	"read_weight" : "5",
	"write_weight" : "5",
	"cacheline_interleaving" : 0, #bank interleaving
	})

link_nvm_bus_link = sst.Link("NVM_bus_link")
link_nvm_bus_link.connect((NVM_controller, "bus", "60ps"), (slow_memory, "cube_link", "60ps"))


# Connect L3 to mem controller
#L3Link = sst.Link("L3Link")
#L3Link.connect((l3, "directory", "60ps"), (memController, "port" + str(l3cache_net_addr), "60ps"))

# Connect DRAM to mem controller
DRAM_dc_link = sst.Link("DRAM_dc_link")
DRAM_dc_link.connect((DRAM, "direct_link", "60ps"), (DRAM_dc, "memory", "60ps"))
DRAM_net_link = sst.Link("DRAM_dc_net_link")
DRAM_net_link.connect((DRAM_dc, "network", "60ps"), (memController, "port" + str(DRAM_dc_port), "60ps"))

# Connect (slow) NVM to mem controller
slow_dc_link = sst.Link("slow_dc_link")
slow_dc_link.connect((slow_memory, "direct_link", "60ps"), (slow_dc, "memory", "60ps")) # originally direct_link
slow_net_link = sst.Link("slow_dc_net_link")
slow_net_link.connect((slow_dc, "network", "60ps"), (memController, "port" + str(slow_dc_port), "60ps"))

memController.addParam("num_ports", rtrInfo.numPorts())

 #Enable SST Statistics Outputs for this simulation
#sst.setStatisticLoadLevel(16)
sst.enableAllStatisticsForAllComponents({"type":"sst.AccumulatorStatistic"})

#sst.setStatisticOutput("sst.statOutputCSV")

#if len(sys.argv) > 5:
#output_file = "STATS/test.csv"
#else:
#       output_file = "STATS/"+str(exe) + "-b" + str(num_banks) + "-r" + str(num_rows) + "".join(opts) + ".csv"

#sst.setStatisticOutputOptions( {
#         "filepath"  : output_file,
#         "separator" : ", "
#} )


print "Finished SST Configuration!"
