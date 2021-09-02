const { MessageEmbed } = require("discord.js");
const config = require("../../configs/token.json");
const chnl = require("../../configs/chnl.json");
const resource = require("../../configs/resource.json");
const superagent = require("superagent");
const colors = require("../../configs/colors.json");
module.exports = {
    config: {
        name: `marsweather`,
        category: "",
        description: "",
        aliases: [`marsw`],
    },
    run: async(bot, message, args) => {
        try {
            let getMarsData = async() => {
                let result = await fetch(
                    "https://api.nasa.gov/insight_weather/?api_key=6xN4Xyh4nluXEufxFnT0fLzku2lEgBUYbv3Orttc"
                );
                let json = await result.json();
                return json;
            };
            let marsDataVal = await getMarsData();
            let sol = marsDataVal.sol_keys[6];
            let lastestTemp = marsDataVal[sol].AT.av;
            msg.reply(
                "Latest temperature from NASA's InSight Mars Lander at Elysium Planitia: " +
                lastestTemp +
                "°C"
            );
        } catch (err) {
            const embed = new MessageEmbed()
                .setTitle("Whoops, looks like something went wrong!")
                .setThumbnail(resource.aw_snap)
                .setDescription(
                    "Use `" +
                    config.prefix +
                    "help` for a list of avaliable commands or use `" +
                    config.prefix +
                    "support` to join the support server!"
                )
                .setFooter("Still facing issues? Join the support server!");
            message.channel.send({ embed }).then((m) => {
                m.delete({ timeout: 5000 });
            });
        }
    },
};