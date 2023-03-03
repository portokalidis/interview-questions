public int trap(int[] height) {
  int trappedWater = 0;
  int leftIndex = 0;
  int rightIndex = height.length - 1;
  int leftMax = 0;
  int rightMax = 0;
  while(leftIndex < rightIndex) {
    if (height[leftIndex] <= height[rightIndex]) {
      leftMax = Math.max(leftMax, height[leftIndex]);
      trappedWater += Math.max(0, leftMax - height[leftIndex]);
      leftIndex += 1;
    }
    else {
      rightMax = Math.max(rightMax, height[rightIndex]);
      trappedWater += Math.max(0, rightMax - height[rightIndex]);
      rightIndex -= 1;
    }
  }
  return trappedWater;
}
    
