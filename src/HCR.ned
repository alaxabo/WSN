//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

simple BaseStation
{
    parameters:
        int xpos;
        int ypos;
        int xMax;
        int yMax;
        int k;
        double trRange;
        @display("p=$xpos,$ypos;i=device/antennatower");
    gates:
        input in[];
        output out[];
}

simple Sensor
{
    parameters:
        int xpos;
        int ypos;
        double energy @unit("nJ");
        double trRange;
        @display("p=$xpos,$ypos;i=misc/node_vs;is=vs");
    gates:
        input in[];
        output out[];
}

network HCR
{
    parameters:
        int totalNodes;
        int k;
        double trRange;
        int xMax;
        int yMax;
        double energy @unit("nJ");
        @display("bgs=2");
    submodules:
        nodes[totalNodes]: Sensor {
            parameters:
                xpos = intuniform(0, 100);
                ypos = intuniform(0, 100);
                trRange = trRange;
                energy = energy;
        }

        sink: BaseStation {
            parameters:
                xpos = 400;
                ypos = 800;
                k = k;
                trRange = trRange;
                xMax = xMax;
                yMax = yMax;
        }
    connections allowunconnected:
}
