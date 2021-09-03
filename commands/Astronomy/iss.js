const { MessageEmbed } = require("discord.js");
const config = require("../../configs/token.json");
const chnl = require("../../configs/chnl.json");
const resource = require("../../configs/resource.json");
const superagent = require("superagent");
module.exports = {
    config: {
        name: `iss`,
        category: "",
        description: "",
        aliases: [`internationalspacestation`, `spacestation`],
    },
    run: async(bot, message, args) => {
        try {
            const { body } = await superagent.get(
                "https://api.wheretheiss.at/v1/satellites/25544"
            );
            const embed = new MessageEmbed()
                .setTitle("International Space Station")
                .setDescription("Here you will find info regarding the ISS")
                .addField("Latitude", body.latitude)
                .addField("Longitude", body.longitude)
                .addField("Altitude", body.altitude)
                .addField("Velocity", body.velocity)
                .addField("Solar Latitude", body.solar_lat)
                .addField("Solar Longitude", body.solar_lon)
                .setFooter("All units regarding distance is using Kilometers")
                .setThumbnail(resource.iss);

            message.channel.send({ embed });
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
