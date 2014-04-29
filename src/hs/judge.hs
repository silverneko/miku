import System.Process
import System.IO
import Control.Parallel
import System.Posix.IO
import Data.Maybe
import Control.Concurrent
import Control.Monad
import System.Environment

judge id = do
   let command = ["--box-id="++(show id)]
   (_, _, _, _) <- createProcess(proc "./isolate/isolate" command)
   --(_, _, _, _) <- createProcess(proc "echo" command)
   return ()

main = do
   args <- getArgs
   mapM_ (forkIO . judge) args
