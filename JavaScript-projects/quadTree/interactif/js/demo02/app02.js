//Quadtree : demo insertion and inspection of the depth
//Author : Paul Schuhmacher
import NodePedagogic from '../pedagogic_extension.js';
import {Node, CircularProbe, Point} from '../quadTree.js';
import {uiApp02} from './ui_app02.js';
import * as apiP5 from '../api_render.js';


export default function demo02(sketch){

  let rootNode;
  let circularProbe;

  sketch.setup = function() {

    console.log('setup demo02');

    sketch.createCanvas(1024, 600);
    sketch.background(0);

    rootNode = new NodePedagogic(sketch.width / 2, sketch.height / 2, sketch.width / 2, sketch.height / 2);
    circularProbe = new CircularProbe(sketch.width / 2, sketch.height / 2, 250);
  }

  sketch.draw = function() {
    sketch.background(0);
    uiApp02.update();
    circularProbe.moveTo(sketch.mouseX, sketch.mouseY);
    rootNode.depthUnderCursor(circularProbe);
    apiP5.apiP5.showNodeWithDepth(sketch, rootNode);
  }

  //Interace with UI
  sketch.insertNormal = function() {
    rootNode.insert(new Point(circularProbe.x, circularProbe.y));
  }

  sketch.insertGaussian = function(nbPoints) {
    for (let i = 0; i != nbPoints; i++) {
      rootNode.insert(new Point(sketch.randomGaussian(circularProbe.x, circularProbe.r / 3), sketch.randomGaussian(circularProbe.y, circularProbe.r / 3)));
    }
  }

  sketch.reset = function() {
    if (rootNode)
      rootNode.clear();
  }

  sketch.getMaxDepth = function() {
    if (rootNode)
      return rootNode.getMaxDepth();
  }

  sketch.numberOfPoints = function() {
    let n = rootNode.nbPoints();
    return n === undefined ? 0 : n;
  }

}
