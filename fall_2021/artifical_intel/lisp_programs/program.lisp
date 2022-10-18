(defun send(sm sc m c mg cg)
  (terpri)
  (format t "~d  ~d === ~d, ~d ==> ~d  ~d" sm sc m c mg cg))

(defun sendBack(sm sc m c mg cg)
  (terpri)
  (format t "~d  ~d <== ~d, ~d === ~d  ~d" sm sc m c mg cg))

(defun mcAlgo()
  (princ "You've selected Missionaries and Cannibals!")
  (terpri)
  (princ "Please input the number of Missionaries and Cannibals, then the boat size.")
  (terpri)
  (princ "Missionaries and Cannibals: ")
  (setf numMC (read))
  (princ "Boat size: ")
  (setf boatSize (read))
  (setf startC numMC)
  (setf startM numMC)
  (setf goalC 0)
  (setf goalM 0)
  (setf boatC 0)
  (setf boatM 0)
  (princ "Start              Goal")
  (terpri)
  (princ "M   C              M  C")
  (terpri)
  (format t "~d  ~d             ~d  ~d" startM startC goalM goalC)
  ;; Send initial set of cannibals
  (incf boatC boatSize)
  (decf startC boatSize)
  (send startM startC boatM boatC goalM goalC)
  (incf goalC (- boatC 1))
  (decf boatC goalC)
  (sendBack startM startC boatM boatC goalM goalC)
  (incf boatM (- boatSize boatC))
  (decf startM (- boatSize boatC))
  (send startM startC boatM boatC goalM goalC)
  (incf goalM boatM)
  (decf boatM goalM)
  (sendBack startM startC boatM boatC goalM goalC)
  ;; Done with inital setup for problem
  ;; fix the sizing so that if the boat is odd make even num missionaries
  (if (eq (mod boatSize 2) 0)
    (progn
      (setq numMis (/ boatSize 2))
      (setq numCan (/ boatSize 2)))
    (progn
      (setq numMis (/ (- boatSize (mod boatSize 2)) 2))
      (setq numCan (/ (- boatSize (mod boatSize 2)) 2))))
    ; loop while there are more missionaries than cannibals
  (loop while (> startM (- boatSize 1))
    do
      (incf boatM numMis)
      (incf boatC (- numCan boatC))
      (decf startM boatM)
      (decf startC (- numCan 1))
      (send startM startC boatM boatC goalM goalC)
      (incf goalC (- boatC 1))
      (incf goalM boatM)
      (decf boatC (- boatC 1))
      (decf boatM boatM)
      (sendBack startM startC boatM boatC goalM goalC)
      (if (> startM (- boatSize 1))
        (progn
          ; Send a boat of only cannibals to balance sides
          (incf boatC 1)
          (decf startC 1)
          (send startM startC boatM boatC goalM goalC)
          (incf goalC (- boatC 1))
          (decf boatC (- boatC 1))
          (sendBack startM startC boatM boatC goalM goalC))
        (progn
          (incf boatM startM)
          (decf startM boatM)
          (send startM startC boatM boatC goalM goalC)
          (incf goalM boatM)
          (decf boatM boatM)
          (sendBack startM startC boatM boatC goalM goalC))))
  (if (> startM 0)
    (progn
      (incf boatM startM)
      (decf startM boatM)
      (send startM startC boatM boatC goalM goalC)
      (incf goalM boatM)
      (decf boatM boatM)
      (sendBack startM startC boatM boatC goalM goalC))
    nil)
  ; Send remaining cannibals
  (incf boatC startC)
  (decf startC (- boatC 1))
  (send startM startC boatM boatC goalM goalC)
  (incf goalC boatC)
  (setf boatC 0)
  (terpri)
  (format t "~d  ~d === END === ~d  ~d" startM startC goalM goalC))



(defun isFeasible (current)
  (setf count 0)
  (defparameter tempList ())
  (loop for i in current
    append(loop for j in i
            do(push j tempList)))
  (setf tempList (reverse tempList))
  (loop for i from 0 to 7
    append(loop for j from (+ i 1) to 8
            do(if (and (and (not (eq (nth i tempList) 'E)) (not (eq (nth j tempList) 'E))) (> (nth i tempList) (nth j tempList)))
                (incf count 1))))
  (if (eq (mod count 2) 0)
    (return-from isFeasible T)
    (return-from isFeasible nil)))


(defun getPos(current)
  (loop for i from 0 to 2
    append(loop for j from 0 to 2
            do(if (eq (nth j (nth i current)) 'E)
                (return-from getPos (cons i j))
                nil))))

(defun createState(x y eX eY current)
  (if (and (>= x 0) (< x 3))
    nil
    (return-from createState nil))
  (if (and (>= y 0) (< y 3))
    nil
    (return-from createState nil))
  (setq retState (copy-tree current))
  (setf temp (nth y (nth x retState)))
  (setf (nth y (nth x retState)) (nth eY (nth eX retState)))
  (setf (nth eY (nth eX retState)) temp)
  (if (isFeasible retState)
    (return-from createState retState)
    nil))

(defun generateState(current goal previous)
  ; For each possible movement of E at current state
  ; find the min h(n) value. Since its generating
  ; at the same depth, no need to include g(n)
  (terpri)
  (printState current)
  (setf ePos (getPos current))
  ; Move/Validate (x + 1, y) (x - 1, y) (x, y + 1) (x, y - 1)
  (setf x (first ePos))
  (setf y (rest ePos))
  (setq upState (createState (- x 1) y x y current))
  (setq downState (createState (+ x 1) y x y current))
  (setq leftState (createState x (- y 1) x y current))
  (setq rightState (createState x (+ y 1) x y current))
  (setq up (distance upState goal))
  (setq down (distance downState goal))
  (setq left (distance leftState goal))
  (setq right (distance rightState goal))
  (setq h (min up down left right))
  (if (eq up 0)
    (return-from generateState upState)
    (if (eq down 0)
      (return-from generateState downState)
      (if (eq left 0)
        (return-from generateState leftState)
        (if (eq right 0)
          (return-from generateState rightState)
          nil))))

  (if (and (eq h up) (not(eq (distance upState previous) 0)))
    (generateState upState goal current)
    (if (and (eq h down) (not(eq (distance downState previous) 0)))
     (generateState downState goal current)
     (if (and (eq h left) (not(eq (distance leftState previous) 0)))
       (generateState leftState goal current)
       (if (and (eq h right) (not(eq (distance rightState previous) 0)))
         (generateState rightState goal current)
         nil)))))

(defun distance(current goal)
  ; check to see if current state is in goal configuration
  (if (not goal)
    (return-from distance nil)
    nil)
  (setq hn 0)
  (if (eq (first (first current)) (first (first goal)))
    nil
    (incf hn 1))
  (if (eq (second (first current)) (second (first goal)))
    nil
    (incf hn 1))
  (if (eq (third (first current)) (third (first goal)))
    nil
    (incf hn 1))
  (if (eq (first (second current)) (first (second goal)))
    nil
    (incf hn 1))
  (if (eq (second (second current)) (second (second goal)))
    nil
    (incf hn 1))
  (if (eq (third (second current)) (third (second goal)))
    nil
    (incf hn 1))
  (if (eq (first (third current)) (first (third goal)))
    nil
    (incf hn 1))
  (if (eq (second (third current)) (second (third goal)))
    nil
    (incf hn 1))
  (if (eq (third (third current)) (third (third goal)))
    nil
    (incf hn 1))
  hn)

(defun printState(state)
  (princ (first state))
  (terpri)
  (princ (second state))
  (terpri)
  (princ (third state))
  (terpri))

(defun solveEightPuzzle(current goal depth)
  ; Test all paths on this depth by getting h(n) for
  ; each possible movement of E whichever h(n) has the lowest score
  ; call solveEightPuzzle again with the new state at that h(n)
  (if (eq (distance current goal) 0)
    (return-from solveEightPuzzle current)
    nil)
  ; (princ depth)
  (princ "Start State:")
  (setq results (generateState current goal nil))
  results)
  ; (solveEightPuzzle (generateState nextState goal current) goal (+ depth 1)))

(defun eightPuzzle()
  (princ "You've selected 8-Puzzle")
  (terpri)
  (princ "Enter a custom start state?")
  (terpri)
  (princ "Yes (1) or No (0): ")
  (setq yesno (read))
  (if (eq yesno 0)
    (setf startState '((E 1 3) (4 2 5) (7 8 6)))
    (princ "Please Enter Start State: "))
  (if (eq yesno 1)
    (setf startState (read))
    nil)
  (if (not (isFeasible startState))
    (progn
      (princ "Infeasiable puzzle, please restart and retry.")
      (terpri)
      (return-from eightPuzzle nil))
    nil)
  (setf goalState '((1 2 3) (4 5 6) (7 8 E)))
  (setf completed (solveEightPuzzle startState goalState 0))
  (princ "Final State: ")
  (terpri)
  (printState completed))

(defun start()
  (princ "Please Select a Problem: Missionaries and Cannibals (Enter 1) or 8-Puzzle (Enter 2)")
  (terpri)
  (princ "Selection: ")
  (setq selection (read))
  (if (eq selection 1)
    (mcAlgo)
    (eightPuzzle)))

(start)
