const { MessageEmbed } = require('discord.js');
const config = require('../../configs/config.json');
const chnl = require('../../configs/chnl.json');
const resource = require('../../configs/resource.json');
const superagent = require('superagent');
const colors = require('../../configs/colors.json');
module.exports = {
    config: {
        name: `calcsun`,
        category: '',
        description: '',
        aliases: [``]
    },
    run: async (bot, message, args) => {
    try {
        let lat = args[0], lon = args[1], time = args[2];
        if(!lat || !lon || lat == undefined || lon == undefined) {
            const embed = new MessageEmbed()
            .setTitle("Oops, looks like you forgot some parameters...")
            .setThumbnail(resource.aw_snap)
            .addField("Description", "This command is used to get sunset and sunrise times for a specific longtitude & latitude.")
            .addField("Usage", "`"+config.prefix+"calcsun {latitude} {longitude} {time[OPTIONAL]}`")
            .addField("Example Usage", "`"+config.prefix+"calcsun 36.7201600 -4.4203400`")
            .addField("{time[OPTIONAL]}", "If nothing is supplied, it will be automatically entered as `today`, otherwise use date formatting: `YYYY-MM-DD`")
            .setFooter("Still have questions? Join the support server")
            .setColor(colors.error)

            message.channel.send({embed})
        } else if (!time || time == undefined){
            var {body} = await superagent.get(`https://api.sunrise-sunset.org/json?lat=${lat}&lng=${lon}&date=today`);
            const embed = new MessageEmbed()
            .setTitle("Sunrise-Sunset for: Today")
            .addField("Entered Coordinates", "**Longitude:** `"+lon+"`\n**Latitude:** `"+lat+"`")
            .addField("Sunrise", "`"+body.results.sunrise+"`")
            .addField("Sunset", "`"+body.results.sunset+"`")
            .addField("Solar Noon", body.results.solar_noon)
            .addField("Length of Day", body.results.day_length)
            .addField("Start of Civil Twilight", body.results.civil_twilight_begin)
            .addField("End of Civil Twilight", body.results.civil_twilight_end)
            .addField("Start of Nautical Twilight", body.results.nautical_twilight_begin)
            .addField("End of Nautical Twilight", body.results.nautical_twilight_end)
            .addField("Start of Astronomical Twilight", body.results.astronomical_twilight_begin)
            .addField("End of Astronomical Twilight", body.results.astronomical_twilight_end)

            message.channel.send({embed})
        } else {
            var {body} = await superagent.get(`https://api.sunrise-sunset.org/json?lat=${lat}&lng=${lon}&date=${time}`)

                const embed = new MessageEmbed()
                .setTitle("Sunrise-Sunset for: "+time)
                .addField("Entered Coordinates", "**Longitude:** `"+lon+"`\n**Latitude:** `"+lat+"`")
                .addField("Sunrise", "`"+body.results.sunrise+"`", true)
                .addField("Sunset", "`"+body.results.sunset+"`", true)
                .addField("Solar Noon", body.results.solar_noon, true)
                .addField("Length of Day", body.results.day_length, true)
                .addField("Start of Civil Twilight", body.results.civil_twilight_begin, true)
                .addField("End of Civil Twilight", body.results.civil_twilight_end, true)
                .addField("Start of Nautical Twilight", body.results.nautical_twilight_begin, true)
                .addField("End of Nautical Twilight", body.results.nautical_twilight_end, true)
                .addField("Start of Astronomical Twilight", body.results.astronomical_twilight_begin, true)
                .addField("End of Astronomical Twilight", body.results.astronomical_twilight_end, true)

                message.channel.send({embed})
        }
    } catch (err) {
        bot.channels.cache.get(chnl.errors).send(err.stack)
        const embed = new MessageEmbed()
        .setTitle("404 Bad Request")
        .setThumbnail(resource.aw_snap)
        .setDescription("Use `"+config.prefix+"help` for a list of avaliable commands or use `"+config.prefix+"support` to join the support server!")
        .addField("I could not find anything based on the entered value", "Check the parameters entered")
        .setFooter("Still facing issues? Join the support server!")
        message.channel.send({ embed }).then(m => {
            m.delete({ timeout: 5000 })
        });
    }
  }
  }