//Maxwell King-Wilson
//n00824138

import java.io.*;
import java.util.*;
import java.lang.Math;
import java.util.List;
import java.util.Stack;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Iterator;


public class Project3{




    public static void main(String[] args) throws IOException {
        int pFrames = Integer.parseInt(args[0]);
        int pNumber = 10000;
        int jobSize;
        int pRange = Integer.parseInt(args[1]);
        Random rn = new Random();
        jobSize = Math.abs(rn.nextInt() % pNumber);
        int[] set = new int[pNumber];

        for(int i = 0; i < set.length; i++){
            int page = Math.abs(rn.nextInt() % pRange) + 0;
            set[i] = page;
            //System.out.println(set[i]);

        }

        //set = new int[]{7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
        //set = new int[]{2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2};

        //System.out.println("FIFO: ");
        int fifoFaults = fifo(pFrames, set);
        //System.out.println("LRU: ");
        int lruFaults = lru(pFrames, set);
        int optimalFaults = optimal(pFrames, set);
        int clockFaults = clock(pFrames, set);
        System.out.println("Numbers generated: " + pNumber);
        System.out.println("\t\ta\t\tb\t\tPF");
        System.out.println("FCFS: \t\t" + pFrames + "\t\t" + pRange + "\t\t" +  fifoFaults);
        System.out.println("LRU: \t\t" + pFrames + "\t\t" + pRange + "\t\t" + lruFaults);
        System.out.println("Optimal: \t" + pFrames + "\t\t" + pRange + "\t\t" + optimalFaults);
        System.out.println("Clock: \t\t" + pFrames + "\t\t" + pRange + "\t\t" + clockFaults);
    }

    public static int fifo(int frames, int[] set){

        int[] slots = new int[frames];
        int   faults = 0, initPlace = 0;
        //2 3 2 1 5 2 4 5 3 2 5 2
        //set = new int[]{2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2};
        for(int i = 0; i < slots.length; i++){
            slots[i] = -1;
        }

        Queue<Integer> fifoQ = new LinkedList<Integer>();

        //if fifoQ contains the set number, continue.  else if fifoQ does not contain it AND is not full add it to the next slot. if not contained AND full, add to fault count, remove the top entry to the fifoQ and replace the matching number to the frame
        for(int i = 0; i < set.length; i++){
            //do nothing if the number is already in the set
            if(fifoQ.contains(set[i])){
                continue;
            } else {
                if(fifoQ.size() < frames){
                    //while there are still empty frames, add a fault every time an empty frame is filled
                    fifoQ.add(set[i]);
                    slots[initPlace] = set[i];
                    initPlace++;
                    faults++;

                } else {
                    int match;
                    for(int j = 0; j < slots.length; j++){
                        if(fifoQ.peek() == slots[j]){
                            //remove the oldest number and replace it with the newest,  keep record of what the newest and oldest numbers are using a queue
                            fifoQ.poll();
                            fifoQ.add(set[i]);
                            slots[j] = set[i];
                            //System.out.println(Arrays.toString(slots));
                            break;
                        }
                    }
                    faults++;
                }
            }
            //System.out.println("end pass: " + Arrays.toString(slots));
        }


        //System.out.println(fifoQ);
        //System.out.println("FIFO Final: " + Arrays.toString(slots));

        return faults;
    }

    public static int lru(int frames, int[] set){
        int[] slots = new int[frames];
        int   faults = 0, initPlace = 0;

        //2 3 2 1 5 2 4 5 3 2 5 2
        //set = new int[]{2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2};
        //set = new int[]{7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};

        for(int i = 0; i < slots.length; i++){
            slots[i] = -1;
        }

        Queue<Integer> initQ = new LinkedList<Integer>();
        List<Integer> lruQ = new ArrayList<Integer>();

        for(int i = 0; i < set.length; i++){
            //System.out.println("looking at: " + set[i]);

            if(lruQ.contains(set[i])){
                //System.out.println("Found: " + set[i]);
                //System.out.println("Before: " + lruQ);
                lruQ.remove(Integer.valueOf(set[i]));
                lruQ.add(set[i]);

                //System.out.println("After: " + lruQ);
            } else {
                //System.out.println("Adding: " + set[i]);
                lruQ.add(set[i]);

            }

            boolean match = false;

            for(int j = 0; j < slots.length; j++){
                if(set[i] == slots[j]){
                    match = true;
                }
            }

            if(match){
                continue;
            } else {
                if((initQ.size() < frames)){//while there are still empty frames, add a fault every time an empty frame is filled
                    initQ.add(set[i]);
                    slots[initPlace] = set[i];
                    initPlace++;
                    faults++;
                } else {
                    //lruQ.indexOf()  Returns the index of the first occurrence of the specified element in this list, or -1 if this list does not contain the element. More formally, returns the lowest index i such that (o==null ? get(i)==null : o.equals(get(i))), or -1 if there is no such index.
                    //lruQ.indexOf(set[i]);

                    //from the start of lruQ, check to see if the number is located in the slots.  if so replace that slot with set[i] then break out of the loop

                    for(int j = 0; j < lruQ.size(); j++){
                        int cc = lruQ.get(j);
                        boolean found = false;
                        for(int k = 0; k < slots.length; k++){
                            if(slots[k] == cc){
                                //replace oldest number with the new number
                                //System.out.println("YAT");
                                slots[k] = set[i];
                                faults++;
                                found = true;
                                break;
                            }
                        }
                        if(found){
                            break;
                        }
                    }

                }
            }
            //System.out.println("end pass: " + Arrays.toString(slots));
        }
        
        for(int i = 0; i < slots.length; i++){
           
        }
        return faults;
    }

    public static int optimal(int frames, int[] set){
        int[] slots = new int[frames];
        int   faults = 0, initPlace = 0;




        for(int i = 0; i < slots.length; i++){
            slots[i] = -1;
        }

        Queue<Integer> initQ = new LinkedList<Integer>();


        for(int i = 0; i < set.length; i++){
            //System.out.println("looking at: " + set[i]);
            int place = i;
            List<Integer> optQ = new ArrayList<Integer>();



            boolean match = false;

            //look for matches

            for(int j = 0; j < slots.length; j++){
                if(set[i] == slots[j]){
                    match = true;
                }
            }


            if(match){
                continue;
            } else {
                if((initQ.size() < frames)){//while there are still empty frames, add a fault every time an empty frame is filled
                    initQ.add(set[i]);
                    slots[initPlace] = set[i];
                    initPlace++;
                    faults++;
                } else {
                    //look into the future and find the number in frames that is the furthest out to replace
                    int[] ages = new int[frames];
                    for(int j = 0; j < slots.length; j++){
                        //any number not found in the future is givin a number larger than the length of set
                        ages[j] = set.length+100;
                        for(int k = place; k < set.length; k++){
                            //if matched assign that number an age for comparison
                            if(slots[j] == set[k]){
                                ages[j] = (k-place);
                                break;
                            }
                        }
                    }
                    int replace = 0;
                    int age = 0;
                    //find the number to replace, and replace
                    for(int kk = 0; kk < ages.length; kk++){
                        if(ages[kk] > age){
                            age = ages[kk];
                            replace = kk;
                        }
                    }
                    slots[replace] = set[i];

                    // System.out.println("DOOF");
                    faults++;
                }
            }
            //System.out.println("end pass: " + Arrays.toString(slots));
        }
        //System.out.print("OPT Final: ");
        for(int i = 0; i < slots.length; i++){
            //System.out.print(slots[i] + ",");
            //System.out.println();
        }
        return faults;
    }



    public static int clock(int frames, int[] set){
        //set = new int[]{2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2};
        int pointer = 0;
        int[] slots = new int[frames];
        int[] state = new int[frames];
        int   faults = 0, initPlace = 0;



        //2 3 2 1 5 2 4 5 3 2 5 2
        //set = new int[]{2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2};
        //set = new int[]{7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};

        for(int i = 0; i < slots.length; i++){
            slots[i] = -1;
        }
        //state 1 is new, 0 is old
        for(int i = 0; i < state.length; i++){
            state[i] = 1;
        }

        Queue<Integer> initQ = new LinkedList<Integer>();

        for(int i = 0; i < set.length; i++){
            //System.out.println("looking at: " + set[i]);
            int place = i;
            List<Integer> optQ = new ArrayList<Integer>();



            boolean match = false;


            //look for the number in current slots
            for(int j = 0; j < slots.length; j++){
                if(set[i] == slots[j]){
                    match = true;
                    //make the matching slot state "new"
                    state[j] = 1;
                }
            }


            if(match){
                continue;
            } else {
                if((initQ.size() < frames)){//while there are still empty frames, add a fault every time an empty frame is filled
                    initQ.add(set[i]);
                    slots[initPlace] = set[i];
                    initPlace++;
                    faults++;
                    pointer++;

                    //System.out.println(pointer);
                } else {
                    if(pointer > frames-1){
                        pointer = 0;
                    }

                    //state 1 is new, 0 is old
                    for(int j = pointer; j <= frames; j++){
                        if(j == frames){
                            j = 0;
                        }
                        //if the number is new,  mark it old
                        if(state[j] == 1){
                            state[j] = 0;
                        } else {
                            //if the number is old,  replace
                            state[j] = 1;
                            slots[j] = set[i];
                            pointer = j + 1;
                            faults++;
                            break;
                        }
                    }
                    //System.out.print("clock step for " + set[i] + ": ");
                    for(int j = 0; j < slots.length; j++){
                        //System.out.print(slots[j] + ",");

                    }    //System.out.println();

                }
            }
        }
        return faults;
    }
}