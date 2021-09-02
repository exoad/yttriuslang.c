//https://api.nasa.gov/planetary/earth/imagery?lon=100.75&lat=1.5&dim=0.15&api_key=6xN4Xyh4nluXEufxFnT0fLzku2lEgBUYbv3Orttc
const { MessageEmbed } = require("discord.js");
const config = require("../../configs/token.json");
const chnl = require("../../configs/chnl.json");
const resource = require("../../configs/resource.json");
const superagent = require("superagent");
module.exports = {
    config: {
        name: `earth`,
        category: "",
        description: "",
        aliases: [``],
    },
    run: async(bot, message, args) => {
        try {
            let lat = args[0];
            let longi = args[1];
            let date = args[2];

            if (!lat || !longi || lat == undefined || longi == undefined) {
                const embed = new MessageEmbed()
                    .setTitle("Oops, you specified incorrect parameters")
                    .addField(
                        "Usage",
                        `\`${config.prefix}earth {latitude} {longitude} {date}\``
                    )
                    .addField(
                        "Additional Info",
                        "1. Date should be in the following format: `YYYY-MM-DD` (Leave blank for today)\n2. If no image appears, that means the specified coordinates does not have any pictures on that day"
                    )
                    .addField(
                        "Example Usage",
                        "`" + config.prefix + "earth 29.78 -95.33 2018-01-01`"
                    )
                    .setFooter("Have questions? Join the support server!");

                message.channel.send({ embed });
            } else if (!date || date == undefined) {
                message.channel.send({
                    files: [{
                        attachment: `https://api.nasa.gov/planetary/earth/imagery?lon=${longi}&lat=${lat}&dim=0.15&api_key=6xN4Xyh4nluXEufxFnT0fLzku2lEgBUYbv3Orttc`,
                        name: "earth.png",
                    }, ],
                });
                message.channel.send(
                    "**Processing image [There is a possibility there is no image of such coordinates]**"
                );
            } else {
                message.channel.send({
                    files: [{
                        attachment: `https://api.nasa.gov/planetary/earth/imagery?lon=${longi}&lat=${lat}&date=${date}&dim=0.15&api_key=6xN4Xyh4nluXEufxFnT0fLzku2lEgBUYbv3Orttc`,
                        name: "earth.png",
                    }, ],
                });
                message.channel.send(
                    "**Processing image [There is a possibility there is no image of such coordinates]**"
                );
            }
        } catch (err) {
            const embed = new MessageEmbed()
                .setTitle(
                    "Whoops, looks like the requested command doesn't exist (yet)!"
                )
                .setThumbnail(resource.aw_snap)
                .setDescription(
                    "Use `" + config.prefix + "help` for a list of avaliable commands!"
                )
                .setFooter("Still facing issues? Join the support server!");
            message.channel.send({ embed }).then((m) => {
                m.delete({ timeout: 5000 });
            });
        }
    },
};